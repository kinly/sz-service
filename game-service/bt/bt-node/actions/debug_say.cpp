#include <behaviortree_cpp/action_node.h>
#include <cstdint>

#include "bt/bt-define.h"
#include "bt/bt-helper.h"

namespace bt::action {

template <class ss_tt>
ss_tt &operator<<(ss_tt &ss, const bt::define::uuid_list &uuids) {
  ss << "[";
  for (const auto &one : uuids) {
    ss << one << ", ";
  }
  ss << "]";
  return ss;
}

template <class ss_tt, class enum_tt, std::enable_if_t<std::is_enum_v<enum_tt>, bool> = true>
ss_tt &operator<<(ss_tt &ss, const enum_tt &e) {
  ss << static_cast<int64_t>(e);
  return ss;
}

template <class tt> class debug_say : public BT::SyncActionNode {
public:
  debug_say(const std::string &name, const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {BT::InputPort<std::string>("msg"), BT::InputPort<tt>("in")};
  }

private:
  BT::NodeStatus tick() override {
    const auto message = getInput<std::string>("msg");
    if (!message) {
      return BT::NodeStatus::FAILURE;
    }
    const auto value = getInput<tt>("in");
    if (!value) {
      std::cout << "debug_say ERROR: [" << message.value() << "] "
                << value.error() << std::endl;
      return BT::NodeStatus::FAILURE;
    }
    std::cout << "debug_say: [" << message.value() << "] " << value.value()
              << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(debug_say<tt>);
};

// BT_REGISTER_DECLARE_NODE(debug_say<int32_t>, "say_int")
// BT_REGISTER_DECLARE_NODE(debug_say<uint32_t>, "say_uint")
BT_REGISTER_DECLARE_NODE(debug_say<int64_t>, "say.long")
BT_REGISTER_DECLARE_NODE(debug_say<uint64_t>, "say.ulong")
BT_REGISTER_DECLARE_NODE(debug_say<std::string>, "say.string")
// BT_REGISTER_DECLARE_NODE(debug_say<owner_data::ptr>, "say_owner_data")

BT_REGISTER_DECLARE_NODE(debug_say<bt::define::uuid_list>, "say.uuids");
BT_REGISTER_DECLARE_NODE(debug_say<bt::define::stage>, "say.stage");
BT_REGISTER_DECLARE_NODE(debug_say<bt::define::animal_type>, "say.animal_type");

}; // namespace bt::action