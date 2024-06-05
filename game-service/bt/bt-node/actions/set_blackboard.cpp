#pragma once
#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class set_blackboard : public BT::SyncActionNode
{
public:
  set_blackboard(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<tt>("in"), BT::OutputPort<tt>("out") };
  }

private:
  BT::NodeStatus tick() override
  {
    const auto value = getInput<tt>("in");
    if(!value)
    {
      return BT::NodeStatus::FAILURE;
    }
    setOutput("out", *value);
    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(set_blackboard<tt>);
  // static auto_register<set_blackboard<tt>> __bt_reg;
};

// BT_REGISTER_DECLARE_NODE(set_blackboard<int32_t>, "set_int");
// BT_REGISTER_DECLARE_NODE(set_blackboard<uint32_t>, "set_uint");
BT_REGISTER_DECLARE_NODE(set_blackboard<int64_t>, "set_long");
// BT_REGISTER_DECLARE_NODE(set_blackboard<uint64_t>, "set_ulong");
BT_REGISTER_DECLARE_NODE(set_blackboard<bool>, "set_bool");
// BT_REGISTER_DECLARE_NODE(set_blackboard<double>, "set_real");
BT_REGISTER_DECLARE_NODE(set_blackboard<std::string>, "set_string");

}; // namespace bt::action