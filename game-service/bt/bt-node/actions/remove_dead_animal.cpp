
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class remove_dead_animal : public BT::SyncActionNode {
public:
  remove_dead_animal(const std::string &name,
                     const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid>(bt::define::room_uuid_key),
        BT::InputPort<bt::define::uuid>(bt::define::player_uuid_key),
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

    const auto sp_player = sp_room->get_player(
        getInput<bt::define::uuid>(bt::define::player_uuid_key_str).value());
    if (sp_player == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    sp_player->remove_dead_animal();

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(remove_dead_animal);
};
}; // namespace bt::action
