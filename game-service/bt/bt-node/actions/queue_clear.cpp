#include <behaviortree_cpp/action_node.h>
#include <cstdint>

#include "battle.h"
#include "bt/bt-helper.h"

namespace bt::action {
template <class tt> class queue_clear : public BT::SyncActionNode {
public:
  queue_clear(const std::string &name, const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::BidirectionalPort<std::deque<tt>>("queue"),
    };
  }

private:
  BT::NodeStatus tick() override {
    auto any_ref = getLockedPortContent("queue");

    if (!any_ref) {
      return BT::NodeStatus::FAILURE;
    }
    if (any_ref->empty()) {
      return BT::NodeStatus::SUCCESS;
    }
    auto *deque_ptr = any_ref->template castPtr<std::deque<tt>>();
    if (!deque_ptr) {
      return BT::NodeStatus::FAILURE;
    }

    deque_ptr->clear();

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_clear<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_clear<sz::battle::bill>, "queue.clear.bill");

} // namespace bt::action