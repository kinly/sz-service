#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class queue_pop_front : public BT::SyncActionNode
{
public:
  queue_pop_front(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::BidirectionalPort<std::deque<tt>>("queue"),
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
    deque_ptr->pop_front();

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_pop_front<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_pop_front<int64_t>, "pop_long");
BT_REGISTER_DECLARE_NODE(queue_pop_front<std::string>, "pop_string");

}  // namespace bt::action