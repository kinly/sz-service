#pragma once
#include <string_view>

#include "animal.h"
#include "battle.h"
#include "player.h"

namespace bt::define {

static constexpr std::string_view out_key = "out";
static const std::string out_key_str = std::string(out_key);

using uuid_type = sz::uuid;
using uuid_list_type = std::deque<uuid_type>;

static constexpr std::string_view room_uuid_key = "room_uuid";
static const std::string room_uuid_key_str = std::string(room_uuid_key);

using stage_type = sz::battle::stage;
static constexpr std::string_view stage_key = "stage";
static const std::string stage_key_str = std::string(stage_key);

static constexpr std::string_view player_uuid_key = "player_uuid";
static const std::string player_uuid_key_str = std::string(player_uuid_key);

static constexpr std::string_view animal_uuid_key = "animal_uuid";
static const std::string animal_uuid_key_str = std::string(animal_uuid_key);

using animal_type = sz::animal::type;
static constexpr std::string_view animal_type_key = "animal_type";
static const std::string animal_type_key_str = std::string(animal_type_key);

using animal_list_type = std::deque<uuid_type>;

}; // namespace bt::define

