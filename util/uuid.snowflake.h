#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <mutex>

namespace util::uuid_snowflake {

class snowflake_nonlock {
 public:
  void lock() {}
  void unlock() {}
};

template <class lock_tt = snowflake_nonlock>
class snowflake {
 private:
  static constexpr int64_t _epoch = 1609459200000L;  // 2021-01-01 00:00:00 UTC
  static constexpr int64_t _machine_id_bits = 5;
  static constexpr int64_t _data_center_id_bits = 5;
  static constexpr int64_t _sequence_bits = 12;

  static constexpr int64_t _max_machine_id = (1 << _machine_id_bits) - 1;
  static constexpr int64_t _max_data_center_id =
      (1 << _data_center_id_bits) - 1;
  static constexpr int64_t _sequence_mask = (1 << _sequence_bits) - 1;

  static constexpr int64_t _machine_id_shift = _sequence_bits;
  static constexpr int64_t _data_center_id_shift =
      _sequence_bits + _machine_id_bits;
  static constexpr int64_t _timestamp_shift =
      _sequence_bits + _machine_id_bits + _data_center_id_bits;

  using time_point = std::chrono::time_point<std::chrono::steady_clock>;
  time_point _start_time_point = std::chrono::steady_clock::now();
  int64_t _start_millisecond =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();

  std::atomic<int64_t> _last_timestamp{-1};
  std::atomic<int64_t> _sequence{0};

  const int64_t _machine_id = 0;
  const int64_t _data_center_id = 0;

  lock_tt _lock;

 public:
  snowflake() = default;

  snowflake(int64_t machine_id, int64_t data_center_id)
      : _machine_id(machine_id), _data_center_id(data_center_id) {
    if (machine_id < 0 || machine_id >= (1 << _machine_id_bits)) {
      throw std::invalid_argument("Machine ID is out of range");
    }
    if (data_center_id < 0 || data_center_id >= (1 << _data_center_id_bits)) {
      throw std::invalid_argument("Data Center ID is out of range");
    }
  }

  int64_t generate() {
    std::lock_guard<lock_tt> lock(_lock);
    auto timestamp = current_milliseconds();
    if (_last_timestamp == timestamp) {
      _sequence = (_sequence + 1) & _sequence_mask;
      if (_sequence == 0) {
        timestamp = wait_next_millisecond(_last_timestamp);
      }
    }
    else {
      _sequence = 0;
    }

    _last_timestamp = timestamp;

    return ((timestamp - _epoch) << _timestamp_shift) |
           (_data_center_id << _data_center_id_shift) |
           (_machine_id << _machine_id_shift) | _sequence;
  }

 private:
  int64_t current_milliseconds() const {
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - _start_time_point);
    return _start_millisecond + diff.count();
  }

  int64_t wait_next_millisecond(int64_t last_timestamp) const {
    int64_t timestamp = current_milliseconds();
    while (timestamp <= last_timestamp) {
      timestamp = current_milliseconds();
    }
    return timestamp;
  }
};

//  the namespace can be simplified by using "using snowflake = util::uuid_snowflake::generate;".
struct generate {
  static snowflake<>& instance() {
    static snowflake<> result;
    return result;
  }

  static snowflake<std::mutex>& instance_mt() {
    static snowflake<std::mutex> result;
    return result;
  }
};

};  // namespace util::uuid_snowflake