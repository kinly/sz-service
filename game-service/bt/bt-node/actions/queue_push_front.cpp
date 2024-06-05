#include <cstdint>
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
template <class tt>
class queue_push_front : public BT::SyncActionNode
{
public:
  queue_push_front(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {
      BT::BidirectionalPort<std::deque<tt>>("queue"),
      BT::InputPort<tt>("value"),
    };
  }

private:
  BT::NodeStatus tick() override
  {
    auto value = getInput<tt>("value").value();

    auto any_ref = getLockedPortContent("queue");

    if(!any_ref)
    {
      return BT::NodeStatus::FAILURE;
    }
    if(any_ref->empty())
    {
      std::deque<tt> que;
      que.push_back(value);
      any_ref.assign(que);
      return BT::NodeStatus::SUCCESS;
    }
    auto* deque_ptr = any_ref->template castPtr<std::deque<tt>>();
    if(!deque_ptr)
    {
      return BT::NodeStatus::FAILURE;
    }
    deque_ptr->push_front(value);

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_push_front<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_push_front<int64_t>, "push_front_long");
BT_REGISTER_DECLARE_NODE(queue_push_front<std::string>, "push_front_string");

}  // namespace bt::action