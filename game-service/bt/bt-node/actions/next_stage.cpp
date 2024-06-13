
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class next_stage : public BT::SyncActionNode {
public:
  next_stage(const std::string &name, const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid>(bt::define::room_uuid_key),
        BT::OutputPort<bt::define::stage>(bt::define::out_key),
    };
  }

private:
  BT::NodeStatus tick() override {
    const auto op_value =
        getInput<bt::define::uuid>(bt::define::room_uuid_key_str);
    if (!op_value) {
      return BT::NodeStatus::FAILURE;
    }
    const auto sp_room =
        sz::battle::room_mgr::thread_local_inst().get_room(op_value.value());
    if (sp_room == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    const auto next_stage = sp_room->next_stage();
    sp_room->set_stage(next_stage);
    setOutput<bt::define::stage>(bt::define::out_key_str, sp_room->get_stage());

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(next_stage);
};
}; // namespace bt::action
