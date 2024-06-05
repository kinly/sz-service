
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
class clear_last_target : public BT::SyncActionNode
{
public:
  clear_last_target(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::BidirectionalPort<owner_data::ptr>(owner_data::key_name_sv) };
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

    setOutput(owner_data::key_name, *value);
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(clear_last_target);
};
}; // namespace bt::action
