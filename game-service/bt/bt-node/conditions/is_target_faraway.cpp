
#include <behaviortree_cpp/condition_node.h>

#include "bt/bt-helper.h"

namespace bt::condition {
class is_target_faraway : public BT::ConditionNode {
public:
  is_target_faraway(const std::string &name,
                    const BT::NodeConfiguration &config)
      : BT::ConditionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::BidirectionalPort<owner_data::ptr>(owner_data::key_name_sv),
        BT::InputPort<int64_t>("invalid_distance"),
    };
  }

private:
  BT::NodeStatus tick() override {
    const auto value = getInput<owner_data::ptr>(owner_data::key_name);
    if (!value) {
      return BT::NodeStatus::FAILURE;
    }
    const auto owner = value.value()->get_owner();
    if (owner == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(is_target_faraway);
};
}; // namespace bt::condition
