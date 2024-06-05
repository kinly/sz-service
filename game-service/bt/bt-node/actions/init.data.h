#pragma once
#include <behaviortree_cpp/action_node.h>

#include "bt/bt-helper.h"

namespace bt::action {

template <class tt> class init_data : public BT::SyncActionNode {

private:
  tt _data;

public:
  init_data(const std::string &name, const BT::NodeConfiguration &config)
      : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() { return {BT::OutputPort<tt>("data")}; }

  void initialize(const tt &src) { _data = src; }

  void initialize(tt &&src) { _data = std::move(src); }

private:
  BT::NodeStatus tick() override {
    setOutput("data", _data);
    return BT::NodeStatus::SUCCESS;
  }

  BT_DECLARE_NODE(init_data<tt>);
};

}; // namespace bt::action