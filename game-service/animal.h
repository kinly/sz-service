#pragma once
#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "battle.h"
#include "property.h"

namespace sz {

class animal : public std::enable_shared_from_this<animal> {
private:

  std::unordered_map<property::key, property> _properties;

  std::unordered_map<battle::step, battle::executes> _step_execs;
  std::unordered_map<battle::event, battle::executes> _event_execs;

public:
  animal() = default;
  ~animal() = default;

  const property &get_property(const property::key& key) const {
    const auto &iter = _properties.find(key);
    if (iter == _properties.cend())
      return property::invalid;
    return iter->second;
  }

  property &get_property(const property::key &key) {
    const auto &iter = _properties.find(key);
    if (iter == _properties.cend())
      return property::invalid;
    return iter->second;
  }

  bool is_dead() const {
    const auto &iter = _properties.find(property::enum_key::hp);
    if (iter == _properties.cend())
      return true;
    return iter->second.get_value() <= 0;
  }
};
}; // namespace sz