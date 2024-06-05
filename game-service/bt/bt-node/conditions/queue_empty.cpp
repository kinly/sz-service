#include <cstdint>
#include <behaviortree_cpp/condition_node.h>

#include "bt/bt-helper.h"

namespace bt::condition
{
template <class tt>
class queue_empty : public BT::ConditionNode
{
public:
  queue_empty(const std::string& name, const BT::NodeConfiguration& config)
    : BT::ConditionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::BidirectionalPort<std::deque<tt>>("queue"),
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

    return deque_ptr->empty() ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
  }

  BT_DECLARE_NODE(queue_empty<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_empty<int64_t>, "long_queue_empty");
BT_REGISTER_DECLARE_NODE(queue_empty<std::string>, "string_queue_empty");

}  // namespace bt::condition