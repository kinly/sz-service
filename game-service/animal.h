#pragma once
#include <list>
#include <memory>
#include <unordered_map>

#include "defines.h"
#include "property.h"
#include "snowflake.h"
#include "test_helper.h"

namespace sz {

  class animal : public std::enable_shared_from_this<animal> {
public:

  using type = helper::animal_type;

private:
  const uuid _uuid = invalid_uuid;
  const type _type = type::unknown;

  std::unordered_map<property::key, property> _properties;

public:
  animal(uuid id, type ty) : _uuid(id), _type(ty) {}
  ~animal() = default;

  static std::shared_ptr<animal> create(type ty) {
    const auto &config_property_iter = helper::animal_configure.find(ty);
    if (config_property_iter == helper::animal_configure.cend())
      return nullptr;

    uuid id = uuid_generator::inst_mt().nextid();
    std::shared_ptr<animal> result = std::make_shared<animal>(id, ty);
    for (const auto &one : config_property_iter->second.properties) {
      result->_properties.emplace(one.first, property{one.first, one.second});
    }
    return result;
  }

  uuid get_uuid() const { return _uuid; }
  type get_type() const { return _type; }

  const property &get_property(const property::key& key) const {
    static const property invalid{};
    const auto &iter = _properties.find(key);
    if (iter == _properties.cend())
      return invalid;
    return iter->second;
  }

  property &get_property(const property::key &key) {
    static property invalid{};
    const auto &iter = _properties.find(key);
    if (iter == _properties.cend())
      return invalid;
    return iter->second;
  }

  /// @brief get and create property
  property &gc_property(const property::key &key) {
    const auto it = _properties.emplace(key, property(key, property::default_value));
    return it.first->second;
  }

  bool is_dead() const {
    const auto &iter = _properties.find(property::enum_key::prop_hp);
    if (iter == _properties.cend())
      return true;
    return iter->second.get_value() <= 0;
  }
};
}; // namespace sz