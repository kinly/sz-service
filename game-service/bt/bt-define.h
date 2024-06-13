#pragma once
#include <string_view>

#include "animal.h"
#include "battle.h"
#include "player.h"

namespace bt::define {

static constexpr std::string_view out_key = "out";
static const std::string out_key_str = std::string(out_key);

using uuid = sz::uuid;
using uuid_list = std::deque<uuid>;

static constexpr std::string_view room_uuid_key = "room_uuid";
static const std::string room_uuid_key_str = std::string(room_uuid_key);

using stage = sz::battle::stage;
static constexpr std::string_view stage_key = "stage";
static const std::string stage_key_str = std::string(stage_key);

static constexpr std::string_view player_uuid_key = "player_uuid";
static const std::string player_uuid_key_str = std::string(player_uuid_key);

static constexpr std::string_view animal_uuid_key = "animal_uuid";
static const std::string animal_uuid_key_str = std::string(animal_uuid_key);

using animal_type = sz::animal::type;
static constexpr std::string_view animal_type_key = "animal_type";
static const std::string animal_type_key_str = std::string(animal_type_key);

using animal_list_type = std::deque<uuid>;

using animal_property = sz::property::key;
static constexpr std::string_view animal_property_key =
    "animal_property";
static const std::string animal_property_key_str =
    std::string(animal_property_key);

using animal_property_value = sz::property::value;
static constexpr std::string_view animal_property_value_key =
    "animal_property_value";
static const std::string animal_property_value_key_str =
    std::string(animal_property_value_key);

using damage_type = sz::battle::damage_type;
static constexpr std::string_view damage_type_key = "damage_type";
static const std::string damage_type_key_str = std::string(damage_type_key);

using damage_value = sz::property::value;
static constexpr std::string_view damage_value_key = "damage_value";
static const std::string damage_value_key_str = std::string(damage_value_key);

using attack_bill = sz::battle::bill;
static constexpr std::string_view attack_bill_key = "attack_bill_data";
static const std::string attack_bill_key_str = std::string(attack_bill_key);

}; // namespace bt::define

