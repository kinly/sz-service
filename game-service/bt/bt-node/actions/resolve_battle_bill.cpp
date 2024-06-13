#include <behaviortree_cpp/action_node.h>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {
class resolve_battle_bill : public BT::SyncActionNode {
public:
  resolve_battle_bill(const std::string &name,
                    const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
        BT::InputPort<bt::define::attack_bill>(bt::define::attack_bill_key),
        BT::OutputPort<bt::define::uuid>("attacker_player"),
        BT::OutputPort<bt::define::uuid>("attacker_animal"),
        BT::OutputPort<bt::define::uuid>("beattacker_player"),
        BT::OutputPort<bt::define::uuid>("beattacker_animal"),
        BT::OutputPort<bt::define::damage_type>(bt::define::damage_type_key),
        BT::OutputPort<bt::define::damage_value>(bt::define::damage_value_key),
    };
  }

private:
  BT::NodeStatus tick() override {
    const auto bill =
        getInput<bt::define::attack_bill>(bt::define::attack_bill_key_str)
            .value();
    setOutput<bt::define::uuid>("attacker_player", bill.attacker_player);
    setOutput<bt::define::uuid>("attacker_animal", bill.attacker_animal);
    setOutput<bt::define::uuid>("beattacker_player", bill.beattacker_player);
    setOutput<bt::define::uuid>("beattacker_animal", bill.beattacker_animal);
    setOutput<bt::define::damage_type>(bt::define::damage_type_key_str,
                                       bill.damage_type);
    setOutput<bt::define::damage_value>(bt::define::damage_value_key_str,
                                        bill.damage);
    return BT::NodeStatus::SUCCESS;
  }

  BT_REGISTER_NODE(resolve_battle_bill);
};

}; // namespace bt::action