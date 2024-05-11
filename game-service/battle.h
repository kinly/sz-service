#pragma once
#include <list>
#include <string>
#include <thread>

namespace sz {

class player;
class animal;

namespace battle {

enum class step : uint16_t {
  start = 0,

  round_start = 1,
  attack_start = 2,
  attack_end = 3,
  round_end = 4,

  end = 5,
};

enum class event : uint16_t {
  animal_born,
  animal_dead,
  animal_attack,
  animal_be_attack,
};

enum class execute_type : uint16_t {

};

class execute {
private:
  std::string _exec;
  std::weak_ptr<animal> _master;
  std::weak_ptr<player> _player;

};

class executes {
private:
  std::list<execute> _execs;
};

  
class room {
public:
  using uuid = int64_t;
private:
  uuid _uuid;

  const std::thread::id _work_thread;

  std::shared_ptr<player> _master;
  std::shared_ptr<player> _slaver;

  uint32_t _round = 0;
  uint32_t _max_round = 0;
};

class room_mgr {
private:
  std::unordered_map<room::uuid, room> _rooms;
};

}; // namespace battle
}; // namespace sz
