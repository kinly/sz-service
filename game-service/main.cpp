#include <iostream>
#include <chrono>

#include "cinatra.hpp"

using namespace std::chrono_literals;

struct person_t {
  void foo(cinatra::coro_http_request &, cinatra::coro_http_response &res) {
    res.set_status_and_content(cinatra::status_type::ok, "ok");
  }
};

async_simple::coro::Lazy<void> basic_usage() {
  cinatra::coro_http_server server(std::thread::hardware_concurrency(), 9001);
  server.set_http_handler<cinatra::GET>(
      "/get",
      [](cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
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

int main() {
  async_simple::coro::syncAwait(basic_usage());
  return 0;
}