#include "battle.h"

#include "player.h"
#include "snowflake.h"
#include "nlohmann/json.hpp"

bool sz::battle::room::alert::is_ok() const {
  switch (_evt) {
  case event::alert_dead:
  case event::alert_hp_threshold:
    return _player != nullptr && _animal != nullptr;
  default:
    return false;
  }
}

// {"master": {
//     "uuid": 123, "animals": [],
//   }, "slaver": {
//     "uuid": 123, "animals": [],
//   }
// }
std::shared_ptr<sz::battle::room>
sz::battle::room::create(const std::string_view json_sv) {
  using namespace std::string_view_literals;

  static constexpr std::string_view key_master = "master"sv;
  static constexpr std::string_view key_slaver = "slaver"sv;
  static constexpr std::string_view key_player_uuid = "uuid"sv;

  const nlohmann::json root = nlohmann::json::parse(json_sv, nullptr, false);
  if (root.is_discarded()) {
    return nullptr;
  }

  if (!root.contains(key_master) || !root[key_master].is_object()) {
    return nullptr;
  }
  if (!root.contains(key_slaver) || !root[key_slaver].is_object()) {
    return nullptr;
  }
  const auto &json_master = root[key_master];
  const auto &json_slaver = root[key_slaver];

  uuid id = uuid_generator_mt().nextid();
  auto result = std::make_shared<room>(id, std::this_thread::get_id());

  {
    if (!json_master.contains(key_player_uuid) ||
        !json_master[key_player_uuid].is_number_unsigned()) {
      return nullptr;
    }
    const auto player_uuid = json_master[key_player_uuid].get<player::uuid>();
    const auto player_ptr = player::create(player_uuid, json_master);
    if (!player_ptr) {
      return nullptr;
    }
    result->_master = player_ptr;
  }
  {
    if (!json_slaver.contains(key_player_uuid) ||
        !json_slaver[key_player_uuid].is_number_unsigned()) {
      return nullptr;
    }
    const auto player_uuid = json_slaver[key_player_uuid].get<player::uuid>();
    const auto player_ptr = player::create(player_uuid, json_slaver);
    if (!player_ptr) {
      return nullptr;
    }
    result->_slaver = player_ptr;
  }

  return result;
}

void sz::battle::room::run_operator() {
  std::shared_ptr<player> next_player = _master;

  while (_evt != event::end) {
    for (const auto &one : _alerts) {
      if (!one.is_ok())
        continue;

      // one._player->run(one._evt, )
    }

    if (_evt == event::start) {
      _evt = event::beg_round;
    } else if (_evt == event::beg_round) {
      _round += 1;
      if (_round >= _max_round) {
        _evt = event::end;
        continue;
      }
    } else if (_evt == event::beg_attack) {

    } else if (_evt == event::run_attack) {
    } else if (_evt == event::end_attack) {
    } else if (_evt == event::end_round) {
    } 

    // _master->run(_evt, shared_from_this());
  }
}

void sz::battle::room::run(event evt) {
  if (evt == event::start) {
    _evt = event::beg_round;
  } else if (evt == event::beg_round) {
    _round += 1;
    if (_round >= _max_round) {
      _evt = event::end;
      return;
    }
  } else if (evt == event::beg_attack) {
    
  } else if (evt == event::run_attack) {
  } else if (evt == event::end_attack) {
  } else if (evt == event::end_round) {
  } else if (evt == event::alert_dead) {
  } else if (evt == event::end) {
  }

  // _master->run();
  // _slaver->run();
}
