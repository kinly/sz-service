#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class queue_size : public BT::SyncActionNode
{
public:
  queue_size(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::BidirectionalPort<std::deque<tt>>("queue"),
      BT::OutputPort<int64_t>("size"),
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
      setOutput("size", 0);
      return BT::NodeStatus::SUCCESS;
    }
    auto* deque_ptr = any_ref->template castPtr<std::deque<tt>>();
    if(!deque_ptr)
    {
      return BT::NodeStatus::FAILURE;
    }

    setOutput<int64_t>("size", deque_ptr->size());

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_size<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_size<int64_t>, "long_queue_size");
BT_REGISTER_DECLARE_NODE(queue_size<std::string>, "string_queue_size");

}  // namespace bt::action