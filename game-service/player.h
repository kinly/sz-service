#pragma once
#include <list>
#include <memory>
#include "nlohmann/json.hpp"

namespace sz {
  namespace battle {
    class room;
  }

  class animal;

class player : public std::enable_shared_from_this<player> {
public:
  using uuid = std::uint64_t;
  static constexpr uuid invalid_uuid = -1;

private:
  const uuid _uuid = invalid_uuid;
  std::list<std::shared_ptr<animal>> _aliver;
  std::list<std::shared_ptr<animal>> _deader;
  std::uint32_t _slots = 0;

public:
  player(uuid id) : _uuid(id) {}
  ~player() = default;

  static std::shared_ptr<player> create(uuid id, const nlohmann::json& json_data);

  void battle_step(std::shared_ptr<battle::room> sroom);
};

}; // end namespace sz
