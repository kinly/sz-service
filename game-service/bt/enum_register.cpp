#include "bt/bt-helper.h"

#include "battle.h"
#include "animal.h"
#include "property.h"

namespace bt {

struct enum_register {
  BT_ENUM_REGISTER_KEY(sz::battle::stage, sz_battle_stage);

  // BT_ENUM_REGISTER_KEY(sz::battle::event, sz_battle_event);

  BT_ENUM_REGISTER_KEY(sz::animal::type, sz_animal_type);

  BT_ENUM_REGISTER_KEY(sz::property::key, sz_property_type);

  BT_ENUM_REGISTER_KEY(sz::battle::damage_type, sz_damage_type);
};

};