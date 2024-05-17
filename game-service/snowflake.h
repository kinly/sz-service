#pragma once

#include <chrono>
#include <cstdint>
#include <mutex>
#include <stdexcept>

#include "non-lock.h"

namespace sz {

// via: https://github.com/sniper00/snowflake-cpp/blob/master/snowflake.hpp

template <typename lock_tt = nonlock>
class snowflake {
  using lock_type = lock_tt;
  static constexpr int64_t TWEPOCH = 1704038400000;
  static constexpr int64_t WORKER_ID_BITS = 5L;
  static constexpr int64_t DATACENTER_ID_BITS = 5L;
  static constexpr int64_t MAX_WORKER_ID = (1 << WORKER_ID_BITS) - 1;
  static constexpr int64_t MAX_DATACENTER_ID = (1 << DATACENTER_ID_BITS) - 1;
  static constexpr int64_t SEQUENCE_BITS = 12L;
  static constexpr int64_t WORKER_ID_SHIFT = SEQUENCE_BITS;
  static constexpr int64_t DATACENTER_ID_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS;
  static constexpr int64_t TIMESTAMP_LEFT_SHIFT =
      SEQUENCE_BITS + WORKER_ID_BITS + DATACENTER_ID_BITS;
  static constexpr int64_t SEQUENCE_MASK = (1 << SEQUENCE_BITS) - 1;

  using time_point = std::chrono::time_point<std::chrono::steady_clock>;

  time_point start_time_point_ = std::chrono::steady_clock::now();
  int64_t start_millisecond_ =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();

  int64_t last_timestamp_ = -1;
  int64_t worker_id_ = 0;
  int64_t datacenter_id_ = 0;
  int64_t sequence_ = 0;
  mutable lock_type lock_;

public:
  snowflake() = default;

  snowflake(const snowflake &) = delete;

  snowflake &operator=(const snowflake &) = delete;

  void init(int64_t worker_id, int64_t datacenter_id) {
    if (worker_id > MAX_WORKER_ID || worker_id < 0) {
      throw std::runtime_error(
          "worker Id can't be greater than 31 or less than 0");
    }

    if (datacenter_id > MAX_DATACENTER_ID || datacenter_id < 0) {
      throw std::runtime_error(
          "datacenter Id can't be greater than 31 or less than 0");
    }

    worker_id_ = worker_id;
    datacenter_id_ = datacenter_id;
  }

  int64_t nextid() {
    std::lock_guard<lock_type> lock(lock_);
    // std::chrono::steady_clock  cannot decrease as physical time moves forward
    auto timestamp = millisecond();
    if (last_timestamp_ == timestamp) {
      sequence_ = (sequence_ + 1) & SEQUENCE_MASK;
      if (sequence_ == 0) {
        timestamp = wait_next_millis(last_timestamp_);
      }
    } else {
      sequence_ = 0;
    }

    last_timestamp_ = timestamp;

    return ((timestamp - TWEPOCH) << TIMESTAMP_LEFT_SHIFT) |
           (datacenter_id_ << DATACENTER_ID_SHIFT) |
           (worker_id_ << WORKER_ID_SHIFT) | sequence_;
  }

private:
  int64_t millisecond() const noexcept {
    const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time_point_);
    return start_millisecond_ + diff.count();
  }

  int64_t wait_next_millis(int64_t last) const noexcept {
    auto timestamp = millisecond();
    while (timestamp <= last) {
      timestamp = millisecond();
    }
    return timestamp;
  }
};

static snowflake<> &uuid_generator() {
  static snowflake<> inst;
  return inst;
}

static snowflake<std::mutex> &uuid_generator_mt() {
  static snowflake<std::mutex> inst;
  return inst;
}

}; // namespace sz