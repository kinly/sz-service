#pragma once

namespace util {

template <class tt, bool is_thread_local = false> class singleton {
public:
  singleton() = default;
  virtual ~singleton() = default;

  // non-copyable
  singleton(const singleton &) = delete;
  singleton &operator=(const singleton &) = delete;
  singleton(singleton &&) = delete;
  singleton &operator=(singleton &&) = delete;

public:
  static inline tt &instance() {
    if constexpr (is_thread_local) {
      static thread_local tt inst{};
      return inst;
    }
    static tt inst{};
    return inst;
  }
};
}; // namespace util