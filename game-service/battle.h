#pragma once
#include <list>
#include <string>
#include <thread>
#include <unordered_map>
#include <behaviortree_cpp/bt_factory.h>

#include "defines.h"

#include "player.h"

namespace sz {
class animal;

namespace battle {

class room;

enum class event : uint16_t {
  start = 0,             // battle start (exchange event -> beg_round)

  beg_round = 1,         // begin round (round + 1 & max-round check)
  beg_attack,            // begin attack
  run_attack,            // running attack logic
  end_attack,            // end attack
  end_round,             // end current round

  alert_dead,            // alert: animal dead
  alert_hp_threshold,    // alert: animal sub-hp
  alert_animal_onstage,  // alert: new(summon) animal onstage

  end,                   // battle end
};

  enum class stage : uint16_t {
    start = 1,         // 战斗开始
    start_round,       // 回合开始
    pre_attack,        // 攻击前
    attack,            // 攻击
    final_attack,      // 结算
    end_attack,        // 攻击结束
    dead,              // 死亡
    end_round,         // 回合结束
    end,               // 战斗结束
  };

class execute {
private:
  std::string _exec;

public:
  execute(const std::string_view sv) : _exec(sv) {}

  void run(std::shared_ptr<room> sroom, std::shared_ptr<player> splayer,
           std::shared_ptr<animal> sanimal) {
    
  }
};

class room : public std::enable_shared_from_this<room> {
public:
  struct alert {
    event _evt;
    std::shared_ptr<player> _player;
    std::shared_ptr<animal> _animal;

    explicit alert(event evt, std::shared_ptr<player> p = nullptr,
                   std::shared_ptr<animal> a = nullptr)
        : _evt(evt), _player(std::move(p)), _animal(std::move(a)) {}

    bool is_ok() const;
  };
private:
  const uuid _uuid;

  BT::Tree _bt_tree;

  const std::thread::id _work_thread;

  std::shared_ptr<player> _master;
  std::shared_ptr<player> _slaver;

  event _evt = event::start;
  stage _stage = stage::start;

  std::uint32_t _round = 0;
  std::uint32_t _max_round = 10;

  std::vector<alert> _alerts;

public:
  room(uuid id, BT::Tree bt_tree, std::thread::id tid,
       std::uint32_t max_round = 10)
      : _uuid(id), _bt_tree(std::move(bt_tree)), _work_thread(tid),
        _max_round(max_round) {}

  ~room() = default;

  static std::shared_ptr<room> create(const std::string_view bt_tree, const std::string_view json_sv);

  uuid get_uuid() const { return _uuid; }

  stage get_stage() const { return _stage; }

  void set_stage(stage src) { _stage = src; }

  std::shared_ptr<player> get_master() const { return _master; }
  std::shared_ptr<player> get_slaver() const { return _slaver; }

  std::shared_ptr<player> get_player(uuid puid) const {
    if (_master && _master->get_uuid() == puid) {
      return _master;
    }
    if (_slaver && _slaver->get_uuid() == puid) {
      return _slaver;
    }
    return nullptr;
  }

  void run_bt();

  void run_operator();

  void run(event evt);
};

class room_mgr final {
private:

  std::unordered_map<uuid, std::shared_ptr<room>> _rooms;

public:
  room_mgr() = default;
  ~room_mgr() = default;

  bool insert(const std::shared_ptr<room>& room) {
    return _rooms.emplace(room->get_uuid(), room).second;
  }

  std::shared_ptr<room> get_room(uuid uid) const {
    if (const auto it = _rooms.find(uid); it != _rooms.end()) {
      return it->second;
    }
    return nullptr;
  }

  static room_mgr& thread_local_inst() {
    static thread_local room_mgr inst;
    return inst;
  }

  void run() {

  }
};

}; // namespace battle
}; // namespace sz
