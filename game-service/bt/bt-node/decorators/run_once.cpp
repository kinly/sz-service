#include <string>
#include <behaviortree_cpp/decorator_node.h>

#include "bt/bt-helper.h"

namespace bt::decorators
{
class run_once : public BT::DecoratorNode
{
  bool m_child_running = false;

  bool m_executed = false;

public:

  run_once(const std::string& name, const BT::NodeConfiguration& config)
    : BT::DecoratorNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return {};
  }

private:
  BT::NodeStatus tick() override
  {
    if(m_executed)
    {
      return BT::NodeStatus::SUCCESS;
    }

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

    setStatus(BT::NodeStatus::RUNNING);

    const BT::NodeStatus child_state = child_node_->executeTick();
    m_child_running = child_state == BT::NodeStatus::RUNNING;
    if(m_child_running)
    {
      return BT::NodeStatus::RUNNING;
    }

    if(isStatusCompleted(child_state))
    {
      haltChild();
    }

    if(child_state == BT::NodeStatus::FAILURE)
    {
      return BT::NodeStatus::FAILURE;
    }

    m_executed = true;

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(run_once);
};

}; // bt::decorators