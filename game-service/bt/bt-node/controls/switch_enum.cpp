#include <behaviortree_cpp/control_node.h>
#include <type_traits>

#include "bt/bt-helper.h"

#include "animal.h"
#include "battle.h"

// via: controls/switch_node.h
namespace bt::controls {
template <class enum_tt, std::size_t count_vv,
          std::enable_if_t<std::is_enum_v<enum_tt>, bool> = true>
class switch_enum : public BT::ControlNode {
private:
  int running_child_;
  std::vector<std::string> case_keys_;

public:
  switch_enum(const std::string &name, const BT::NodeConfig &config)
      : BT::ControlNode::ControlNode(name, config), running_child_(-1) {
    for (std::size_t i = 1; i <= count_vv; ++i) {
      case_keys_.emplace_back(std::string("case_") + std::to_string(i));
    }
  }

  virtual ~switch_enum() override = default;

  void halt() override {
    running_child_ = -1;
    ControlNode::halt();
  }

  static BT::PortsList providedPorts() {
    static BT::PortsList result = []() {
      BT::PortsList ports;
      ports.insert(BT::InputPort<enum_tt>("variable"));
      for (std::size_t i = 1; i <= count_vv; ++i) {
        ports.insert(
            BT::InputPort<enum_tt>(std::string("case_") + std::to_string(i)));
      }
      return ports;
    }();
    return result;
  }

private:
  virtual BT::NodeStatus tick() override {
    if (childrenCount() != count_vv + 1) {
      throw BT::LogicError("Wrong number of children in switch_enum; "
                           "must be (num_cases + default)");
    }

    enum_tt variable;
    enum_tt value;
    int match_index = int(count_vv); // default index;

    // no variable? jump to default
    if (getInput("variable", variable)) {
      // check each case until you find a match
      for (int index = 0; index < int(count_vv); ++index) {
        const std::string &case_key = case_keys_[index];
        if (getInput(case_key, value)) {
          if (variable == value) {
            match_index = index;
            break;
          }
        }
      }
    }

    // if another one was running earlier, halt it
    if (running_child_ != -1 && running_child_ != match_index) {
      haltChild(running_child_);
    }

    const auto &selected_child = children_nodes_[match_index];
    const BT::NodeStatus ret = selected_child->executeTick();
    if (ret == BT::NodeStatus::SKIPPED) {
      // if the matching child is SKIPPED, should I jump to default or
      // be SKIPPED myself? Going with the former, for the time being.
      running_child_ = -1;
      return BT::NodeStatus::SKIPPED;
    } else if (ret == BT::NodeStatus::RUNNING) {
      running_child_ = match_index;
    } else {
      resetChildren();
      running_child_ = -1;
    }
    return ret;
  }

  static auto_register<switch_enum<enum_tt, count_vv>> __bt_reg;
};

template <>
auto_register<switch_enum<sz::battle::stage, 9>>
    switch_enum<sz::battle::stage, 9>::__bt_reg{"switch.enum.battle.stage"};

template <>
auto_register<switch_enum<sz::animal::type, 12>>
    switch_enum<sz::animal::type, 12>::__bt_reg{"switch.enum.animal.type"};

template <>
auto_register<switch_enum<sz::battle::damage_type, 2>>
    switch_enum<sz::battle::damage_type, 2>::__bt_reg{
        "switch.enum.damage.type"};

}; // namespace bt::controls

