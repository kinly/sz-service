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
  std::unordered_map<battle::event, std::string_view> events;
};

static inline std::unordered_map<animal_type, configure> animal_configure;

static bool init_configure() {
  configure rat;
  rat.type = animal_type::rat;

  rat.properties.emplace_back(property::enum_key::hp, 1);
  rat.properties.emplace_back(property::enum_key::attack, 1);

  rat.events.emplace(battle::event::start, "start");
  rat.events.emplace(battle::event::beg_round, "beg_round");
  rat.events.emplace(battle::event::beg_attack, "beg_attack");
  rat.events.emplace(battle::event::run_attack, "run_attack");
  rat.events.emplace(battle::event::end_attack, "end_attack");
  rat.events.emplace(battle::event::end_round, "end_round");

  animal_configure.emplace(rat.type, rat);

  return true;
}

static inline const bool _init_configure_ = init_configure();

} // namespace helper
} // namespace sz
