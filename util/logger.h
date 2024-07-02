//
//  easy_logger.h
//  inlay
//
//  Created by Kinly on 2022/11/26.
//

#pragma once

#ifndef NOMINMAX
#undef min
#undef max
#endif

#include <spdlog/async.h>
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <sstream>
#include <thread>

/// spdlog wrap class
namespace util::logger {

class easy_logger_static {
public:
  static void init() {
    constexpr std::size_t log_buffer_size = 32 * 1024; // 32kb
    spdlog::init_thread_pool(log_buffer_size,
                             std::thread::hardware_concurrency());
  }

  // will drop all register logger and shutdown
  static void shutdown() { spdlog::shutdown(); }

  // spdlog static globally
  static auto level() -> decltype(spdlog::get_level()) {
    return spdlog::get_level();
  }

  static void set_level(spdlog::level::level_enum lvl) {
    spdlog::set_level(lvl);
  }

  static bool should_log(spdlog::level::level_enum lvl) {
    return spdlog::should_log(lvl);
  }

  static void set_flush_on(spdlog::level::level_enum lvl) {
    spdlog::flush_on(lvl);
  }

  static const char *get_shortname(const std::string &path) {
    if (path.empty())
      return path.data();

    const size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
      return path.data() + 0;
    return path.data() + (pos + 1);
  }
};

/// let easy_logger like stream
class log_line {
private:
  std::ostringstream _ss;

public:
  log_line() noexcept {}

  template <class _Ty> log_line &operator<<(const _Ty &src) {
    _ss << src;
    return *this;
  }
  std::string str() const { return _ss.str(); }
};

class log_stream {
private:
  spdlog::source_loc _loc;
  spdlog::level::level_enum _lvl = spdlog::level::info;

public:
  log_stream(const spdlog::source_loc &loc, spdlog::level::level_enum lvl)
      : _loc(loc), _lvl(lvl) {}
  bool operator==(const log_line &ll) const {
    spdlog::log(_loc, _lvl, "{}", ll.str());
    return true;
  }
};

class easy_logger final : public easy_logger_static {
public:
private:
  std::atomic_bool _inited;

public:
  static easy_logger &get() {
    static easy_logger easy_logger;
    return easy_logger;
  }

  bool init(std::string_view filename) {

    if (_inited.load())
      return true;

    try {
      // initialize spdlog
      std::vector<spdlog::sink_ptr> sinks;
      sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(
          filename.data(), 0, 2));

      // constexpr std::size_t max_file_size = 1024l * 1024 * 1024; // 1G
      // sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      //     filename.data(), max_file_size, 1024));

#if !defined(WIN32) && !defined(NO_CONSOLE_LOG)
      sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif

#if defined(_DEBUG) && defined(WIN32) && !defined(NO_CONSOLE_LOG)
      sinks.push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif //  _DEBUG

      // register logger
      spdlog::set_default_logger(
          std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end()));

      spdlog::set_pattern("[%Y-%m-%d %T.%e][%l][%@][%P:%t]:%v");
      spdlog::flush_on(spdlog::level::warn);
      spdlog::set_level(spdlog::level::trace);
      spdlog::flush_every(std::chrono::seconds(3));

      spdlog::set_error_handler([](const std::string &msg) {
        spdlog::log(spdlog::level::critical, "*** LOGGER ERROR ***: {}", msg);
      });

    } catch (const std::exception_ptr &e) {
      assert(false);
      return false;
    }

    _inited.store(true);

    return true;
  }

  template <typename... args_tt>
  void log(const spdlog::source_loc &loc, spdlog::level::level_enum lvl,
           const char *fmt, const args_tt &...args) {
    spdlog::log(loc, lvl, fmt, args...);
  }

  template <typename... args_tt>
  void print(const spdlog::source_loc &loc, spdlog::level::level_enum lvl,
             const char *fmt, const args_tt &...args) {
    spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
  }

private:
  easy_logger() = default;
  ~easy_logger() = default;

  easy_logger(const easy_logger &) = delete;
  void operator=(const easy_logger &) = delete;
};
} // namespace util::logger

#include "nostd_source_location.h"

// // got short filename(exclude file directory)
// #define __FILENAME__
// (util::logger::easy_logger_static::get_shortname(__FILE__))

// default
// use fmt lib, e.g. LOG_TRACE("warn log, {1}, {1}, {2}", 1, 2);
#define LOG_TRACE(msg, ...)                                                    \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::trace)) {  \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::trace, msg, ##__VA_ARGS__);                           \
    }                                                                          \
  };
#define LOG_DEBUG(msg, ...)                                                    \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::debug)) {  \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::debug, msg, ##__VA_ARGS__);                           \
    }                                                                          \
  };
#define LOG_INFO(msg, ...)                                                     \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::info)) {   \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::info, msg, ##__VA_ARGS__);                            \
    }                                                                          \
  };
#define LOG_WARN(msg, ...)                                                     \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::warn)) {   \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::warn, msg, ##__VA_ARGS__);                            \
    }                                                                          \
  };
#define LOG_ERROR(msg, ...)                                                    \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::err)) {    \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::err, msg, ##__VA_ARGS__);                             \
    }                                                                          \
  };
#define LOG_CRIT(msg, ...)                                                     \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(                          \
            spdlog::level::critical)) {                                        \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().log(                                    \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::critical, msg, ##__VA_ARGS__);                        \
    }                                                                          \
  };

// use like sprintf, e.g. PRINT_TRACE("warn log, %d-%d", 1, 2);
#define PRINT_TRACE(msg, ...)                                                  \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::trace)) {  \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::trace, msg, ##__VA_ARGS__);                           \
    }                                                                          \
  };
#define PRINT_DEBUG(msg, ...)                                                  \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::debug)) {  \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::debug, msg, ##__VA_ARGS__);                           \
    }                                                                          \
  };
#define PRINT_INFO(msg, ...)                                                   \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::info)) {   \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::info, msg, ##__VA_ARGS__);                            \
    }                                                                          \
  };
#define PRINT_WARN(msg, ...)                                                   \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::warn)) {   \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::warn, msg, ##__VA_ARGS__);                            \
    }                                                                          \
  };
#define PRINT_ERROR(msg, ...)                                                  \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(spdlog::level::err)) {    \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::err, msg, ##__VA_ARGS__);                             \
    }                                                                          \
  };
#define PRINT_CRIT(msg, ...)                                                   \
  {                                                                            \
    if (util::logger::easy_logger_static::should_log(                          \
            spdlog::level::critical)) {                                        \
      constexpr nostd::source_location ns_sl =                                 \
          nostd::source_location::current(                                     \
              __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE(),        \
              __builtin_COLUMN());                                             \
      util::logger::easy_logger::get().print(                                  \
          {ns_sl.file_name(), ns_sl.line(), ns_sl.function_name()},            \
          spdlog::level::critical, msg, ##__VA_ARGS__);                        \
    }                                                                          \
  };