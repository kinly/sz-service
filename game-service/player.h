#pragma once
#include "nlohmann/json.hpp"
#include <list>
#include <memory>

#include "defines.h"

namespace sz {
  class animal;

namespace battle {
  class room;
}; // namespace battle

class player : public std::enable_shared_from_this<player> {

private:
  const uuid _uuid = invalid_uuid;
  std::unordered_map<uuid, std::shared_ptr<sz::animal>> _aliver;
  std::unordered_map<uuid, std::shared_ptr<sz::animal>> _deader;

public:
  player(uuid id) : _uuid(id) {}
  ~player() = default;

  static std::shared_ptr<sz::player> create(uuid id,
                                            const nlohmann::json &json_data);

  uuid get_uuid() const { return _uuid; }

  const std::unordered_map<uuid, std::shared_ptr<sz::animal>> &
  get_alive_animals() const {
    return _aliver;
  }
  const std::unordered_map<uuid, std::shared_ptr<sz::animal>> &
  get_dead_animals() const {
    return _deader;
  }

  std::shared_ptr<animal> get_animal(uuid auid) const {
    if (const auto it = _aliver.find(auid); it != _aliver.end()) {
      return it->second;
    }
    return nullptr;
  }
};

}; // end namespace sz
