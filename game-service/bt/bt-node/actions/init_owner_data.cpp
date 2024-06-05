
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action
{
class init_owner_data : public BT::SyncActionNode
{
  reference_nocopy<bt::owner_data::ptr>& m_nocopy;

public:
  init_owner_data(const std::string& name, const BT::NodeConfiguration& config,
                  bt::reference_nocopy<bt::owner_data::ptr>& nocopy)
    : BT::SyncActionNode(name, config), m_nocopy(nocopy)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::OutputPort<owner_data::ptr>(owner_data::key_name_sv) };
  }

private:
  BT::NodeStatus tick() override
  {
    setOutput(owner_data::key_name, m_nocopy.value());

    return BT::NodeStatus::SUCCESS;
  }

  /// @brief auto register ref
  /// @return just for __reg_self
  static bool self_register()
  {
    helper::factory().registerNodeType<init_owner_data>(
        "init.owner",
        std::ref(bt::reference_nocopy<bt::owner_data::ptr>::s_reference()));

    return true;
  }
  inline static bool __reg_self = self_register();
};
};  // namespace bt::action
