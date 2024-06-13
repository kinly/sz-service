#pragma once
#include <list>
#include <string>
#include <thread>
#include <unordered_map>
#include <behaviortree_cpp/bt_factory.h>

#include "defines.h"
#include "property.h"

namespace sz {
  class player;
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
    attack_bill,       // 结算
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

enum class damage_type : uint32_t {
  damage_normal,
  damage_fanshang,
};

struct bill {
  uuid attacker_player = invalid_uuid;
  uuid attacker_animal = invalid_uuid;
  uuid beattacker_player = invalid_uuid;
  uuid beattacker_animal = invalid_uuid;

  damage_type damage_type = damage_type::damage_normal;
  property::value damage = 0;

  bill() = default;

  bill(uuid ap, uuid aa, uuid bap, uuid baa, enum damage_type dt,
       property::value dv)
      : attacker_player(ap), attacker_animal(aa), beattacker_player(bap),
        beattacker_animal(baa), damage_type(dt), damage(dv) {}

  template <class ss_tt> friend ss_tt &operator<<(ss_tt &ss, const bill &src) {
    ss << "[" << src.attacker_player << ":" << src.attacker_animal
       << "] attack :[" << src.beattacker_player << ":"
       << src.beattacker_animal << "] " << src.damage_type << ":" << src.damage;
    return ss;
  }

  inline static bill from_string(std::string_view sv) {
    
  }

  std::string to_string() {
    
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

  stage next_stage();

  std::shared_ptr<player> get_master() const { return _master; }
  std::shared_ptr<player> get_slaver() const { return _slaver; }

  std::shared_ptr<player> get_player(uuid puid) const;

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
