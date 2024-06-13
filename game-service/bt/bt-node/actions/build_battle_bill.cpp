#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"
#include "bt/bt-define.h"

namespace bt::action {
class build_battle_bill : public BT::SyncActionNode {
public:
  build_battle_bill(const std::string &name,
                    const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::uuid>("attacker_player"),
        BT::InputPort<bt::define::uuid>("attacker_animal"),
        BT::InputPort<bt::define::uuid>("beattacker_player"),
        BT::InputPort<bt::define::uuid>("beattacker_animal"),
        BT::InputPort<bt::define::damage_type>(bt::define::damage_type_key),
        BT::InputPort<bt::define::damage_value>(bt::define::damage_value_key),
        BT::OutputPort<bt::define::attack_bill>(bt::define::out_key),
    };
  }

private:
  BT::NodeStatus tick() override {

    auto bill = bt::define::attack_bill{
        getInput<bt::define::uuid>("attacker_player").value(),
        getInput<bt::define::uuid>("attacker_animal").value(),
        getInput<bt::define::uuid>("beattacker_player").value(),
        getInput<bt::define::uuid>("beattacker_animal").value(),
        getInput<bt::define::damage_type>(bt::define::damage_type_key_str)
            .value(),
        getInput<bt::define::damage_value>(bt::define::damage_value_key_str)
            .value()};

    setOutput(bt::define::out_key_str, bill);
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(build_battle_bill);
};

}; // namespace bt::action