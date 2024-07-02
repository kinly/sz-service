#pragma once
#include <atomic>

namespace util {

/// @brief counter
/// @tparam tt template class type
template <class tt> class counter {
protected:
  static std::size_t _counter_count;
  static std::size_t _counter_alive;

public:
  counter() {
    ++_counter_count;
    ++_counter_alive;
  }

  virtual ~counter() { --_counter_alive; }

  static std::size_t counter_count() { return _counter_count; }
  static std::size_t counter_alive() { return _counter_alive; }
};

template <class tt> std::size_t counter<tt>::_counter_count = 0;
template <class tt> std::size_t counter<tt>::_counter_alive = 0;

/// @brief thread-safe counter
/// @tparam tt template class type
template <class tt> class counter_mt {
protected:
  static std::atomic<std::size_t> _counter_count;
  static std::atomic<std::size_t> _counter_alive;

public:
  counter_mt() {
    ++_counter_count;
    ++_counter_alive;
  }

  virtual ~counter_mt() { --_counter_alive; }

  static std::size_t counter_count() { return _counter_count.load(); }
  static std::size_t counter_alive() { return _counter_alive.load(); }
};

template <class tt> std::size_t counter_mt<tt>::_counter_count{0};
template <class tt> std::size_t counter_mt<tt>::_counter_alive{0};

}; // namespace easy::util
