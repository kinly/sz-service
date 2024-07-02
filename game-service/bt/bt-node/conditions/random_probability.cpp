
#include <behaviortree_cpp/condition_node.h>

#include "bt/bt-helper.h"
#include "random_helper.h"

namespace bt::condition {
class random_probability : public BT::ConditionNode {
public:
  random_probability(const std::string &name,
                     const BT::NodeConfiguration &config)
      : BT::ConditionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<int64_t>("min"),
        BT::InputPort<int64_t>("max"),
        BT::InputPort<int64_t>("probability"),
    };
  }

private:
  BT::NodeStatus tick() override {
    const auto left_value = getInput<int64_t>("min");
    if (!left_value) {
      return BT::NodeStatus::FAILURE;
    }
    const auto right_value = getInput<int64_t>("max");
    if (!right_value) {
      return BT::NodeStatus::FAILURE;
    }
    const auto probability = getInput<int64_t>("probability");
    if (!probability) {
      return BT::NodeStatus::FAILURE;
    }

    return probability.value() <=
                   util::random::range(left_value.value(), right_value.value())
               ? BT::NodeStatus::SUCCESS
               : BT::NodeStatus::FAILURE;
  }

  BT_REGISTER_NODE(random_probability);
};
}; // namespace bt::condition
