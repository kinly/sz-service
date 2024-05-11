#pragma once

namespace sz {

class nonlock {
public:
  void lock() {}
  void unlock() {}
};

class shared_nonlock {
public:
  void lock() {}
  void unlock() {}
  bool try_lock() { return true; }
};
}
