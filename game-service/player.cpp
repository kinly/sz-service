#include "player.h"

#include "animal.h"

// {"animals": [
//     {"type": 1},
//     {"type": 2},
//   ],
//   "animal_slot": 5
// }
std::shared_ptr<sz::player> sz::player::create(uuid id,
                                               const nlohmann::json &json_data) {
  using namespace std::string_view_literals;

  static constexpr std::string_view key_animals = "animals"sv;
  static constexpr std::string_view key_animal_type = "type"sv;
  static constexpr std::string_view key_animal_slot = "animal_slot"sv;

  if (json_data.is_discarded()) {
    return nullptr;
  }

  if (!json_data.contains(key_animal_slot) ||
      !json_data[key_animal_slot].is_number_unsigned()) {
    return nullptr;
  }

  const std::uint32_t slots = json_data[key_animal_slot].get<std::uint32_t>();

  if (!json_data.contains(key_animals) || !json_data[key_animals].is_array()) {
    return nullptr;
  }
  const auto &json_animals = json_data[key_animals];
  if (json_animals.empty() || json_animals.size() > slots) {
    return nullptr;
  }

  auto result = std::make_shared<player>(id);
  result->_slots = slots;

  for (const auto &animal : json_animals) {
    if (!animal.contains(key_animal_type) ||
        !animal[key_animal_type].is_number_unsigned()) {
      continue;
    }
    const auto atype = static_cast<helper::animal_type>(
        animal[key_animal_type].get<std::uint32_t>());
    auto animal_ptr = animal::create(atype);
    if (!animal_ptr) {
      continue;
    }
    result->_aliver.emplace_back(animal_ptr);
  }

  if (result->_aliver.empty()) {
    return nullptr;
  }
  return result;
}

void sz::player::battle_step(std::shared_ptr<battle::room> sroom) {

}
