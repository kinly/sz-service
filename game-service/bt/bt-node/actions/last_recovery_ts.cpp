
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
class last_recovery_ts : public BT::SyncActionNode
{
public:
  last_recovery_ts(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::BidirectionalPort<owner_data::ptr>(owner_data::key_name_sv),
             BT::InputPort<int64_t>("attr"), BT::OutputPort<int64_t>("ts") };
  }

private:
  BT::NodeStatus tick() override
  {
    const auto value = getInput<owner_data::ptr>(owner_data::key_name);
    if(!value)
    {
      return BT::NodeStatus::FAILURE;
    }
    const auto attr_value = getInput<int64_t>("attr");
    if(!attr_value)
    {
      return BT::NodeStatus::FAILURE;
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(last_recovery_ts);
};
};  // namespace bt::action
