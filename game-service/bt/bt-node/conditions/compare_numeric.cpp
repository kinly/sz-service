
#include <behaviortree_cpp/condition_node.h>

#include "bt/bt-helper.h"

namespace bt::condition
{
template <class tt>
class compare_numeric : public BT::ConditionNode
{
public:
  compare_numeric(const std::string& name, const BT::NodeConfiguration& config)
    : BT::ConditionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<tt>("left"),
      BT::InputPort<std::string>("symbol"),
      BT::InputPort<tt>("right"),
    };
  }

private:
  BT::NodeStatus tick() override
  {
    const auto left_value = getInput<tt>("left");
    if(!left_value)
    {
      return BT::NodeStatus::FAILURE;
    }
    const auto right_value = getInput<tt>("right");
    if(!right_value)
    {
      return BT::NodeStatus::FAILURE;
    }
    const auto symbol = getInput<std::string>("symbol");
    if(!symbol)
    {
      return BT::NodeStatus::FAILURE;
    }

    // todo:

    return BT::NodeStatus::SUCCESS;
  }
};
};  // namespace bt::condition
