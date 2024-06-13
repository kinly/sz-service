
#include "battle.h"

namespace bt {
template <class tt> std::string convertToString(tt &&) {
  return std::string{};
}

template <class tt> std::string convertToString(const tt &) {
  return std::string{};
}

template <>
inline std::string convertToString<sz::battle::bill>(const sz::battle::bill &st) {
  return nlohmann::json{
      {"attacker_player", st.attacker_player},
      {"attacker_animal", st.attacker_animal},
      {"beattacker_player", st.beattacker_player},
      {"beattacker_animal", st.beattacker_animal},
      {"damage_type", st.damage_type},
      {"damage_value", st.damage},
  }
      .dump();
}
}; // namespace bt

namespace BT {

template <>
inline sz::battle::bill BT::convertFromString<sz::battle::bill>(StringView str) {
  auto root = nlohmann::json::parse(str, nullptr, false);
  if (root.is_discarded()) {
    throw BT::RuntimeError("invalid input:", str);
  }

  return sz::battle::bill{
      root["attacker_player"]
          .template get<decltype(sz::battle::bill::attacker_player)>(),
      root["attacker_animal"]
          .template get<decltype(sz::battle::bill::attacker_animal)>(),
      root["beattacker_player"]
          .template get<decltype(sz::battle::bill::beattacker_player)>(),
      root["beattacker_animal"]
          .template get<decltype(sz::battle::bill::beattacker_animal)>(),
      root["damage_type"].template get<decltype(sz::battle::bill::damage_type)>(),
      root["damage"].template get<decltype(sz::battle::bill::damage)>(),
  };
}

}; // namespace BT