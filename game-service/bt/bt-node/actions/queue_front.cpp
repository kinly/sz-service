#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class queue_front : public BT::SyncActionNode
{
public:
  queue_front(const std::string& name, const BT::NodeConfiguration& config)
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
      return BT::NodeStatus::FAILURE;  // ?? try to pop from empty queue
    }

    setOutput("value", deque_ptr->front());

    if (getInput<bool>("pop_out").value()) {
      deque_ptr->pop_front();
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_front<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_front<std::string>, "pop_string");

BT_REGISTER_DECLARE_NODE(queue_front<bt::define::uuid>, "queue.front.uuid");

}  // namespace bt::action