#include <iostream>
#include <chrono>
#include "calendar.h"

#include "cinatra.hpp"
#include "redis_helper.h"

#include "battle.h"
#include "datetime.h"

#include "uuid.snowflake.h"
#include "toml++/toml.hpp"

using namespace std::chrono_literals;

struct person_t {
  void foo(cinatra::coro_http_request &, cinatra::coro_http_response &res) {
    res.set_status_and_content(cinatra::status_type::ok, "ok");
  }
};

async_simple::coro::Lazy<void> basic_usage() {
  cinatra::coro_http_server server(std::thread::hardware_concurrency(), 9001);
  server.set_timeout_duration(std::chrono::seconds(10));
  server.set_http_handler<cinatra::GET>(
      "/get",
      [](cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
        resp.set_keepalive(true);
        resp.set_status_and_content(cinatra::status_type::ok, "ok");
        std::cout << "thread-id: " << std::this_thread::get_id() << std::endl;
      });

  server.set_http_handler<cinatra::GET>(
      "/coro",
      [](cinatra::coro_http_request &req,
         cinatra::coro_http_response &resp) -> async_simple::coro::Lazy<void> {
        resp.set_status_and_content(cinatra::status_type::ok, "ok");
        co_return;
      });

  server.set_http_handler<cinatra::GET>(
      "/in_thread_pool",
      [](cinatra::coro_http_request &req,
         cinatra::coro_http_response &resp) -> async_simple::coro::Lazy<void> {
        // will respose in another thread.
        co_await coro_io::post([&] {
          // do your heavy work here when finished work, response.
          resp.set_status_and_content(cinatra::status_type::ok, "ok");
        });
      });

  server.set_http_handler<cinatra::POST, cinatra::PUT>(
      "/post",
      [](cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
        auto req_body = req.get_body();
        resp.set_status_and_content(cinatra::status_type::ok,
                                    std::string{req_body});
      });

  server.set_http_handler<cinatra::GET>(
      "/headers",
      [](cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
        auto name = req.get_header_value("name");
        auto age = req.get_header_value("age");
        assert(name == "tom");
        assert(age == "20");
        resp.set_status_and_content(cinatra::status_type::ok, "ok");
      });

  server.set_http_handler<cinatra::GET>(
      "/query",
      [](cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
        auto name = req.get_query_value("name");
        auto age = req.get_query_value("age");
        assert(name == "tom");
        assert(age == "20");
        resp.set_status_and_content(cinatra::status_type::ok, "ok");
      });

  server.set_http_handler<cinatra::GET, cinatra::POST>(
      "/users/:userid/subscriptions/:subid",
      [](cinatra::coro_http_request &req,
         cinatra::coro_http_response &response) {
        assert(req.params_["userid"] == "ultramarines");
        assert(req.params_["subid"] == "guilliman");
        response.set_status_and_content(cinatra::status_type::ok, "ok");
      });

  person_t person{};
  server.set_http_handler<cinatra::GET>("/person", &person_t::foo, person);

  server.async_start();

  std::this_thread::sleep_for(300ms); // wait for server start

  cinatra::coro_http_client client{};
  auto result = co_await client.async_get("http://127.0.0.1:9001/get");
  assert(result.status == 200);
  assert(result.resp_body == "ok");
  for (auto [key, val] : result.resp_headers) {
    std::cout << key << ": " << val << "\n";
  }

  result = co_await client.async_get("/coro");
  assert(result.status == 200);

  result = co_await client.async_get("/in_thread_pool");
  assert(result.status == 200);

  result = co_await client.async_post("/post", "post string",
                                      cinatra::req_content_type::string);
  assert(result.status == 200);
  assert(result.resp_body == "post string");

  client.add_header("name", "tom");
  client.add_header("age", "20");
  result = co_await client.async_get("/headers");
  assert(result.status == 200);

  result = co_await client.async_get("/query?name=tom&age=20");
  assert(result.status == 200);

  result = co_await client.async_get(
      "http://127.0.0.1:9001/users/ultramarines/subscriptions/guilliman");
  assert(result.status == 200);

  while (true) {
    std::this_thread::sleep_for(10ms);
  }
}

#include "logger.h"

template<class value_tt>
class thread_local_helper final {
private:
  static inline thread_local value_tt _value{};

public:
  thread_local_helper() = default;
  ~thread_local_helper() = default;

  static void set(value_tt &&v) { _value = v; }
  static void set(const value_tt &v) { _value = v; }
  static value_tt get() { return _value; }
};

class dlk_helper final {
  util::redis _redis;

public:
  dlk_helper(std::string_view uri) : _redis(util::redis(uri.data())) {}

  inline bool lock(std::string_view key_sv) {

    static constexpr std::chrono::milliseconds ttl{5000};

    const auto uuid = util::uuid_snowflake::generator::inst_mt().nextid();
    const auto uuid_string = std::to_string(uuid);
    thread_local_helper<std::string>::set(uuid_string);

    return _redis.set(key_sv, uuid_string, ttl, util::redis_utype::NOT_EXIST);
  }

  inline bool unlock(std::string_view key_sv) {
    auto value = thread_local_helper<std::string>::get();

    constexpr std::string_view script =
        "if redis.call('get', KEYS[1]) == ARGV[1] then "
        "  return redis.call('del', KEYS[1]) "
        "else return 0 "
        "end";

    const auto result = _redis.eval<long long>(
        script, std::initializer_list<std::string_view>{key_sv},
        std::initializer_list<std::string_view>{value});
    return 1 == result;
  }
};

class apollo_client {
private:
  cinatra::coro_http_client _http_client{};
  std::atomic_bool _stopped = false;

public:
  async_simple::coro::Lazy<void> waiting_update() {
    while (!_stopped.load()) {
      if (auto result =
        co_await _http_client.async_get("http://127.0.0.1:8080/get"); result.status == 200) {
        std::cout << result.resp_body << std::endl;
      }
    }
  }
};

int main() {
  util::logger::easy_logger::get().init("logs/game-service.log");

  


  auto redis = sw::redis::Redis(
      "redis://123456@127.0.0.1:56379?socket_timeout=50ms&connect_timeout=1s&keep_alive=true");

  std::string_view dlk_key = "123";

  std::pair<int, int> count;

  std::thread t1([dlk_key, &count] {
    dlk_helper dlk(
        "redis://"
        "123456@127.0.0.1:56379?socket_timeout=1s&connect_timeout=1s&keep_alive=true");

    for (int i = 0; i < 1000; ++i) {
      while (dlk.lock(dlk_key) == false)
        std::this_thread::sleep_for(1ms);

      count.first += 1;
      std::cout << "thread 1 locked...." << util::datetime::current_timestamp()
                << " count: " << count.first << ":" << count.second
                << std::endl;
      // std::this_thread::sleep_for(10ms);
      dlk.unlock(dlk_key);
    }
  });

  std::thread t2([dlk_key, &count] {
    dlk_helper dlk(
        "redis://"
        "123456@127.0.0.1:56379?socket_timeout=1s&connect_timeout=1s&keep_alive=true");
    for (int i = 0; i < 1000; ++i) {
      while (dlk.lock(dlk_key) == false)
        std::this_thread::sleep_for(1ms);

      count.second += 1;
      std::cout << "thread 2 locked...." << util::datetime::current_timestamp()
                << " count: " << count.first << ":" << count.second
                << std::endl;
      // std::this_thread::sleep_for(10ms);
      dlk.unlock(dlk_key);
    }
  });

  t1.detach();
  t2.detach();

  try {
    redis.ping();
    redis.set("test", "123");
    std::cout << redis.get("test").value_or("empty") << std::endl;
    std::cout << redis.get("testx").value_or("empty") << std::endl;
  } catch (const std::exception &ex) {
    std::cout << "redis error." << ex.what() << std::endl;
  }

  {
    if (util::logger::easy_logger_static::should_log(spdlog::level::warn)) {
      constexpr nostd::source_location ns_sl = nostd::source_location::current(
          __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),
          __builtin_COLUMN());
      util::logger::easy_logger::get().log(
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},
          spdlog::level::trace, "warn log, {0}-{0}-{1}", 0, 1, 2);
    }
  };
  ;

  LOG_WARN("warn log, {1} wwwww {1}", 1, 2);
  LOG_TRACE("warn log, {0}, {0}, {1}", 1, 2);
  {
    if (util::logger::easy_logger_static::should_log(spdlog::level::trace)) {
      constexpr nostd::source_location ns_sl = nostd::source_location::current(
          __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),
          __builtin_COLUMN());
      util::logger::easy_logger::get().log(
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},
          spdlog::level::trace, "warn log, {}-{}-{}", 0, 1, 2);
    }
  };
  ;

  {
    auto _1 = util::uuid_snowflake::generator::inst_mt().nextid();
    auto _2 = util::uuid_snowflake::generator::inst_mt().nextid();
    auto _3 = util::uuid_snowflake::generator::inst_mt().nextid();

    std::cout << std::endl;
  }

  using namespace std::string_view_literals;
  {
    static constexpr std::string_view animals_key = "animals"sv;

    const auto config = toml::parse_file("../../configs/animals.toml");
    if (!config) {
      std::cout << config.error().description() << std::endl;
      return -1;
    }
    const auto &config_table = config.table();
    if (!config_table.is_table()) {
      return -1;
    }
    for (auto &&[key, value] : config_table) {
      // std::cout << key << std::endl;
      if (!value.is_table()) {
        continue;
      }
    }
  }

  auto sptr_room = sz::battle::room::create(R"(../../configs/bt/root.xml)", R"(
    {
      "master": {
        "uuid": 1,
        "animals": [{"type": 1}, {"type": 1}],
        "animal_slot": 5
      },
      "slaver": {
        "uuid": 2,
        "animals": [{"type": 1}, {"type": 1}, {"type": 1}],
        "animal_slot": 5
      }
    }
  )");

  if (const auto ret = sz::battle::room_mgr::thread_local_inst().insert(sptr_room); ret == false) {
    std::cout << "room insert error" << std::endl;
    return -1;
  }

  sptr_room->run_bt();

  async_simple::coro::syncAwait(basic_usage());
  return 0;
}