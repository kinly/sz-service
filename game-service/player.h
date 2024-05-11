#pragma once
#include <list>
#include <memory>

namespace sz {
class animal;

class player : public std::enable_shared_from_this<player> {
private:
  std::list<std::shared_ptr<animal>> _aliver;
  std::list<std::shared_ptr<animal>> _deader;
};

}; // end namespace sz
