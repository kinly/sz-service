#pragma once

namespace util {

class empty_mutex {
public:
  void lock() {}
  void unlock() {}
};

class empty_shared_mutex {
public:
  void lock() {}
  void unlock() {}
  bool try_lock() { return true; }

  void lock_shared() {}
  void unlock_shared() {}
};

}; // namespace util