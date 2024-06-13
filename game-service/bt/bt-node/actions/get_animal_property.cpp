
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class get_animal_property : public BT::SyncActionNode {
public:
  get_animal_property(const std::string &name,
                         const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid>(bt::define::room_uuid_key),
        BT::InputPort<bt::define::uuid>(bt::define::player_uuid_key),
        BT::InputPort<bt::define::uuid>(bt::define::animal_uuid_key),
        BT::InputPort<bt::define::animal_property>(
            bt::define::animal_property_key),
        BT::BidirectionalPort<bt::define::animal_property_value>(
            bt::define::animal_property_value_key),
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

    const auto sp_animal = sp_player->get_animal(
        getInput<bt::define::uuid>(bt::define::animal_uuid_key_str).value());
    if (sp_animal == nullptr) {
      return BT::NodeStatus::FAILURE;
    }

    const auto &prop =
        sp_animal->gc_property(getInput<bt::define::animal_property>(
                                   bt::define::animal_property_key_str)
                                   .value());

    setOutput<bt::define::animal_property_value>(
        bt::define::animal_property_value_key_str, prop.get_value());

    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(get_animal_property);
};
}; // namespace bt::action