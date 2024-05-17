#pragma once
#include <list>
#include <string>
#include <thread>
#include <unordered_map>

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
  using uuid = int64_t;

  struct alert {
    event _evt;
    std::shared_ptr<player> _player;
    std::shared_ptr<animal> _animal;

    explicit alert(event evt, std::shared_ptr<player> p = nullptr,
                   std::shared_ptr<animal> a = nullptr)
        : _evt(evt), _player(p), _animal(a) {}

    bool is_ok() const;
  };
private:
  const uuid _uuid;

  const std::thread::id _work_thread;

  std::shared_ptr<player> _master;
  std::shared_ptr<player> _slaver;

  event _evt = event::start;
  std::uint32_t _round = 0;
  std::uint32_t _max_round = 10;

  std::vector<alert> _alerts;

public:
  room(uuid id, std::thread::id tid, std::uint32_t max_round = 10)
      : _uuid(id), _work_thread(tid), _max_round(max_round) {}
  ~room() = default;

  static std::shared_ptr<room> create(const std::string_view json_sv);

  void run_operator();

  void run(event evt);
};

class room_mgr {
private:
  std::unordered_map<room::uuid, std::shared_ptr<room>> _rooms;
};

}; // namespace battle
}; // namespace sz
