#pragma once
#include <cstdint>

namespace sz {

class property final {
public:
  using key_type = uint32_t;
  using value_type = int64_t;

  static constexpr value_type default_value = 0;

  enum class enum_key : key_type {
    prop_none,

    prop_hp,
    prop_attack,
    prop_hit_cnt,
    prop_fanshang_cnt,
  };

public:
  using key = enum_key;
  using value = value_type;

public:
  const key _key = key::prop_none;
  value _value = default_value;

public:
  property() = default;
  property(key k, value v) : _key(k), _value(v) {}
  ~property() = default;

  bool is_valid() const {
    return _key == key::prop_none;
  }

  key get_key() const { return _key; }
  value get_value() const { return _value; }

  value set_value(value v) {
    _value = v;
    return get_value();
  }

  value change_value(value v) {
    _value += v;
    return get_value();
  }
};

}; // namespace sz
