#include <string>
#include <behaviortree_cpp/decorator_node.h>

#include "bt/bt-helper.h"

namespace bt::decorators
{
// simple like decorators/loop_node.h
// copied: avoid error: device or resource busy
//-----------------------------------------------------
// eg. error at line string_queue_size
// 
//    <foreach_string queue="{queue}"
//                    value="{element}"
//                    pop_out="true"
//                    if_empty="SUCCESS">
//      <Sequence>
//        <say_string msg="foreach"
//                    in="{element}"/>
//        <string_queue_size queue="{queue}"
//                           size="{size}"/>
//      </Sequence>
//    </foreach_string>
//-----------------------------------------------------
template <class tt>
class queue_foreach_copied : public BT::DecoratorNode
{
  bool m_child_running = false;

public:
  queue_foreach_copied(const std::string& name, const BT::NodeConfiguration& config)
    : BT::DecoratorNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
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
  BT::NodeStatus tick() override
  {
    if(m_child_running)
    {
      const BT::NodeStatus child_state = child_node_->executeTick();
      m_child_running = child_state == BT::NodeStatus::RUNNING;
      if(m_child_running)
      {
        return BT::NodeStatus::RUNNING;
      }
      haltChild();
      if(child_state == BT::NodeStatus::FAILURE)
      {
        return BT::NodeStatus::FAILURE;
      }
    }

    auto deque_v = getInput<std::deque<tt>>("queue");
    if(!deque_v)
    {
      return BT::NodeStatus::FAILURE;
    }

    auto deque_value = deque_v.value();
    if(deque_value.empty())
    {
      return getInput<BT::NodeStatus>("if_empty").value();
    }

    const bool is_pop = getInput<bool>("pop_out").value();

    auto iter = deque_value.begin();
    while(iter != deque_value.end())
    {
      setStatus(BT::NodeStatus::RUNNING);

      // set output first
      setOutput("value", *iter);
      if(is_pop)
      {
        iter = deque_value.erase(iter);
        setOutput("queue", deque_value);
      }
      else
      {
        iter += 1;
      }

      const BT::NodeStatus child_state = child_node_->executeTick();
      m_child_running = child_state == BT::NodeStatus::RUNNING;
      if(m_child_running)
      {
        return BT::NodeStatus::RUNNING;
      }

      haltChild();

      if(child_state == BT::NodeStatus::FAILURE)
      {
        return BT::NodeStatus::FAILURE;
      }
    }

    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(queue_foreach_copied<tt>);
};

BT_REGISTER_DECLARE_NODE(queue_foreach_copied<int64_t>, "foreach_copied_long");
BT_REGISTER_DECLARE_NODE(queue_foreach_copied<std::string>, "foreach_copied_string");

};  // namespace bt::decorators