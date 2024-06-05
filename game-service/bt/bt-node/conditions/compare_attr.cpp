
#include <behaviortree_cpp/condition_node.h>

#include "bt/bt-helper.h"

namespace bt::condition
{
class compare_attr : public BT::ConditionNode
{
public:
  compare_attr(const std::string& name, const BT::NodeConfiguration& config)
    : BT::ConditionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::BidirectionalPort<owner_data::ptr>(owner_data::key_name_sv),
      BT::InputPort<int64_t>("key"),
      BT::InputPort<std::string>("symbol"),
      BT::InputPort<int64_t>("value"),
    };
  }

private:
  BT::NodeStatus tick() override
  {
    const auto value = getInput<owner_data::ptr>(owner_data::key_name);
    if(!value)
    {
      return BT::NodeStatus::FAILURE;
    }
    auto owner = value.value()->get_owner();
    if(owner == nullptr)
    {
      return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(compare_attr);
};
};  // namespace bt::condition
