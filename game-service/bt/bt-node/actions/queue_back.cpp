#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class queue_back : public BT::SyncActionNode
{
public:
  queue_back(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
        BT::BidirectionalPort<std::deque<tt>>("queue"),
        BT::InputPort<bool>("pop_out", true, "pop out?"),
        BT::OutputPort<tt>("value"),
    };
  }

private:
  BT::NodeStatus tick() override
  {
    auto any_ref = getLockedPortContent("queue");

    if(!any_ref)
    {
      return BT::NodeStatus::FAILURE;
    }
    if(any_ref->empty())
    {
      return BT::NodeStatus::SUCCESS;
    }
    auto* deque_ptr = any_ref->template castPtr<std::deque<tt>>();
    if(!deque_ptr)
    {
      return BT::NodeStatus::FAILURE;
    }
    if(deque_ptr->empty())
    {
      return BT::NodeStatus::FAILURE; // ?? try to pop from empty queue
    }

    setOutput("value", deque_ptr->back());
    if (getInput<bool>("pop_out").value()) {
      deque_ptr->pop_back();
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_back<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_back<std::string>, "pop_back_string");

BT_REGISTER_DECLARE_NODE(queue_back<bt::define::uuid>, "queue.back.uuid");

}  // namespace bt::action