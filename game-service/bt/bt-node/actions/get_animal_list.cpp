
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class get_animal_list : public BT::SyncActionNode {
public:
  get_animal_list(const std::string &name, const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid_type>(bt::define::room_uuid_key),
        BT::InputPort<bt::define::uuid_type>(bt::define::player_uuid_key),
        BT::OutputPort<bt::define::animal_list_type>(bt::define::out_key),
    };
  }

private:
  BT::NodeStatus tick() override {
    const auto op_value =
        getInput<bt::define::uuid_type>(bt::define::room_uuid_key_str);
    if (!op_value) {
      return BT::NodeStatus::FAILURE;
    }
    const auto sp_room =
        sz::battle::room_mgr::thread_local_inst().get_room(op_value.value());
    if (sp_room == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    const auto sp_player = sp_room->get_player(
        getInput<bt::define::uuid_type>(bt::define::player_uuid_key_str)
            .value());
    if (sp_player == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    bt::define::animal_list_type out_animals;
    const auto& animals = sp_player->get_alive_animals();
    for (const auto& one : animals) {
      if (one.second == nullptr)
        continue;
      out_animals.emplace_back(one.first);
    }

    setOutput<bt::define::animal_list_type>(bt::define::out_key_str,
                                            out_animals);
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(get_animal_list);
};
}; // namespace bt::action
