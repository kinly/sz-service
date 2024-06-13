
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class get_lowest_property_animal : public BT::SyncActionNode {
public:
  get_lowest_property_animal(const std::string &name,
                             const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid>(bt::define::room_uuid_key),
        BT::InputPort<bt::define::uuid>(bt::define::player_uuid_key),
        BT::InputPort<bt::define::animal_property>(
            bt::define::animal_property_key),
        BT::InputPort<bool>("only_alive", true, "only alive animal?"),
        BT::OutputPort<bt::define::uuid>(bt::define::out_key),
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

    const auto prop_key = getInput<bt::define::animal_property>(
                              bt::define::animal_property_key_str)
                              .value();

    const auto only_alive = getInput<bool>("only_alive").value();

    bt::define::uuid animal_out = sz::invalid_uuid;
    bt::define::animal_property_value animal_out_prop =
        std::numeric_limits<bt::define::animal_property_value>::max();

    const auto &animals = sp_player->get_alive_animals();
    for (const auto &one : animals) {
      if (one.second == nullptr)
        continue;
      if (only_alive && one.second->is_dead()) {
        continue;
      }
      if (const auto new_prop = one.second->get_property(prop_key).get_value();
          animal_out_prop > new_prop) {
        animal_out = one.first;
        animal_out_prop = new_prop;
      }
    }

    setOutput<bt::define::uuid>(bt::define::out_key_str, animal_out);
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(get_lowest_property_animal);
};
}; // namespace bt::action
