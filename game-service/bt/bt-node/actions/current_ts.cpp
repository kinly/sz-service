#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
class current_ts : public BT::SyncActionNode
{
public:
  current_ts(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::OutputPort<timestamp>("ts") };
  }

private:
  BT::NodeStatus tick() override
  {
    setOutput("ts", current_timestamp());
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(current_ts);
};

};  // namespace bt::action