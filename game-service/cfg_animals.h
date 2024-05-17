#pragma once
#include <string_view>
#include <unordered_map>

#include "animal.h"

namespace sz {

class cfg_animals {
public:
  static constexpr std::string_view file = "animals.toml";

private:
  static inline std::unordered_map<std::string_view, animal::type> key2animal{
      {"rat", animal::type::rat},       {"ox", animal::type::ox},
      {"tiger", animal::type::tiger},   {"rabbit", animal::type::rabbit},
      {"dragon", animal::type::dragon}, {"snake", animal::type::snake},
      {"horse", animal::type::horse},   {"sheep", animal::type::sheep},
      {"monkey", animal::type::monkey}, {"rooster", animal::type::rooster},
      {"dog", animal::type::dog},       {"pig", animal::type::pig},
  };

private:

};

}; // namespace sz