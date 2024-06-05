#include <behaviortree_cpp/decorator_node.h>
#include <string>

#include "battle.h"
#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::decorators {
// simple like decorators/loop_node.h
template <class tt> class queue_foreach : public BT::DecoratorNode {
  bool m_child_running = false;

public:
  queue_foreach(const std::string &name, const BT::NodeConfiguration &config)
      : BT::DecoratorNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::BidirectionalPort<std::deque<tt>>("queue"),
        BT::OutputPort<tt>("value"),
        BT::InputPort<bool>("pop_out", false, "pop from queue if it's true"),
        BT::InputPort<BT::NodeStatus>("if_empty", BT::NodeStatus::SUCCESS,
                                      "Status to return if queue is empty: "
                                      "SUCCESS, FAILURE, SKIPPED"),
    };
  }

private:
  BT::NodeStatus tick() override {
    if (m_child_running) {
      const BT::NodeStatus child_state = child_node_->executeTick();
      m_child_running = child_state == BT::NodeStatus::RUNNING;
      if (m_child_running) {
        return BT::NodeStatus::RUNNING;
      }
      haltChild();
      if (child_state == BT::NodeStatus::FAILURE) {
        return BT::NodeStatus::FAILURE;
      }
    }

    auto any_ref = getLockedPortContent("queue");
    if (!any_ref) {
      return BT::NodeStatus::FAILURE;
    }
    if (any_ref->empty()) {
      return getInput<BT::NodeStatus>("if_empty").value();
    }

    auto *deque_ptr = any_ref->template castPtr<std::deque<tt>>();
    if (!deque_ptr) {
      return BT::NodeStatus::FAILURE;
    }

    if (deque_ptr->empty()) {
      return getInput<BT::NodeStatus>("if_empty").value();
    }

    const bool is_pop = getInput<bool>("pop_out").value();

    auto iter = deque_ptr->begin();
    while (iter != deque_ptr->end()) {
      setStatus(BT::NodeStatus::RUNNING);

      // set output first
      setOutput("value", *iter);
      if (is_pop) {
        iter = deque_ptr->erase(iter);
      } else {
        iter += 1;
      }

      const BT::NodeStatus child_state = child_node_->executeTick();
      m_child_running = child_state == BT::NodeStatus::RUNNING;
      if (m_child_running) {
        return BT::NodeStatus::RUNNING;
      }

      haltChild();

      if (child_state == BT::NodeStatus::FAILURE) {
        return BT::NodeStatus::FAILURE;
      }
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_foreach<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_foreach<std::string>, "queue.foreach.string");

BT_REGISTER_DECLARE_NODE(queue_foreach<sz::battle::event>,
                         "queue.foreach.battle.event");

BT_REGISTER_DECLARE_NODE(queue_foreach<bt::define::uuid_type>,
                         "queue.foreach.uuid");

}; // namespace bt::decorators