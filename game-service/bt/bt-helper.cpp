#include "bt-helper.h"

std::optional<BT::Tree>
bt::helper::static_load_tree(const std::string_view key,
                             const std::string_view file) {
  try {

    _factory.registerBehaviorTreeFromFile(file);
    auto bt = _factory.createTree(std::string(key));

    return std::move(bt);

  } catch (const std::exception &e) {
    std::cout << "BT: load behavior-tree xml failed. " << file
              << " msg: " << e.what() << std::endl;
    return {};
  }
}

bool bt::helper::load_tree(const std::string &key, const std::string &file) {
  if (_tree.contains(key)) {
    std::cout << "BT: load tree key repeated. " << key << " file: " << file
              << std::endl;
    return false;
  }

  try {
    _factory.registerBehaviorTreeFromFile(file);
    auto bt = _factory.createTree(key);
    _tree.emplace(key, std::move(bt));
  } catch (const std::exception &e) {
    std::cout << "BT: load behavior-tree xml failed. " << file
              << " msg: " << e.what() << std::endl;
    return false;
  }
  return true;
}

void bt::helper::run(const std::string &key) {
  const auto iter = _tree.find(key);
  if (iter == _tree.end()) {
    std::cout << "BT: run tree not-exist. " << key << std::endl;
    return;
  }

  auto &bt = iter->second;

  bt.tickWhileRunning();
}

bool bt::helper::exist(const std::string &key) const {
  return _tree.contains(key);
}