#pragma once
#include <unordered_map>
#include <vector>

#include "property.h"

namespace sz {
namespace helper {

enum class animal_type : std::uint32_t {
  unknown,

  rat,
  ox,
  tiger,
  rabbit,
  dragon,
  snake,
  horse,
  sheep,
  monkey,
  rooster,
  dog,
  pig
};

struct configure {
  animal_type type;
  std::vector<std::pair<property::key, property::value>> properties;
};

static inline std::unordered_map<animal_type, configure> animal_configure;

static bool init_configure() {
  configure rat;
  rat.type = animal_type::rat;

  rat.properties.emplace_back(property::enum_key::prop_hp, 2000);
  rat.properties.emplace_back(property::enum_key::prop_attack, 1000);

  animal_configure.emplace(rat.type, rat);

  return true;
}

static inline const bool _init_configure_ = init_configure();

} // namespace helper
} // namespace sz
