#pragma once
#include <behaviortree_cpp/bt_factory.h>

#include "singleton.h"

class creature;

namespace bt {
class helper final : public util::singleton<helper> {
protected:
  inline static BT::BehaviorTreeFactory _factory{};

  std::unordered_map<std::string, BT::Tree> _tree;

public:
  helper() = default;
  ~helper() = default;

  static BT::BehaviorTreeFactory &factory() { return _factory; }

  bool load_tree(const std::string &key, const std::string &file);

  void run(const std::string &key);

  bool exist(const std::string &key) const;

  static std::optional<BT::Tree> static_load_tree(const std::string_view key,
                                                  const std::string_view file);
};

template <class tt> class reference_nocopy {
private:
  tt _value{};

public:
  reference_nocopy() {}

  reference_nocopy(const reference_nocopy &) = delete;
  reference_nocopy &operator=(const reference_nocopy &) = delete;

  tt &value() { return _value; }

  void set_value(const tt &src) { _value = src; }

  void set_value(tt &&src) { _value = std::move(src); }

  template <class... args_tt> void set_value(args_tt &&...args) {
    _value = tt(std::forward<args_tt>(args)...);
  }

  static reference_nocopy<tt> &s_reference() {
    static thread_local reference_nocopy<tt> result;
    return result;
  }
};

/// @brief just interface
struct owner_data {
  using ptr = std::shared_ptr<owner_data>;

  inline static constexpr std::string_view key_name_sv = "owner_data_";
  inline static const std::string key_name = std::string(key_name_sv);

public:
  uint64_t m_uuid = 0;

public:
  explicit owner_data(uint64_t uuid) : m_uuid(uuid) {}

  virtual ~owner_data() = default;

  virtual creature *get_owner() const { return nullptr; }

  template <class tt> tt *get_owner() const {
    return dynamic_cast<tt *>(get_owner());
  }
};

template <class node_tt, typename... args_tt> struct auto_register {
  auto_register(const std::string &name, args_tt &&...args) {
    const BT::NodeBuilder builder = [&](const auto &node_name,
                                        const auto &config) {
      return std::make_unique<node_tt>(node_name, config,
                                       std::forward<args_tt>(args)...);
    };

    helper::factory().registerBuilder<node_tt>(name, builder);
  }
};

#define BT_REGISTER_NODE(class_name)                                           \
  inline static auto_register<class_name> __bt_reg{#class_name};

#define BT_DECLARE_NODE(class_name) static auto_register<class_name> __bt_reg;

#define BT_REGISTER_DECLARE_NODE(class_name, key)                              \
  template <> auto_register<class_name> class_name::__bt_reg{key};

template <class enum_tt, std::enable_if_t<std::is_enum_v<enum_tt>, bool> = true>
struct enum_auto_register {
  enum_auto_register() { helper::factory().registerScriptingEnums<enum_tt>(); }
};

#define BT_ENUM_REGISTER(enum_name)                                            \
  inline static enum_auto_register<enum_name> __##enum_name##_bt_reg{};

#define BT_ENUM_REGISTER_KEY(enum_name, key_name)                              \
  inline static enum_auto_register<enum_name> __##key_name##_bt_reg{};

using clock = std::chrono::system_clock;
using default_duration = std::chrono::milliseconds;
using timestamp = std::time_t;

/// @brief current(now) time_point
/// @tparam duration_tt offset duration type
/// @param offset duration value
/// @return time_point (system_clock)
template <typename duration_tt = default_duration>
static constexpr inline clock::time_point
current_time_point(long long offset = 0) noexcept {
  return clock::now() + duration_tt(offset);
}

/// @brief current(now) time_point
/// @tparam req_tt an arithmetic type representing the number of ticks
/// @tparam period_tt a std::ratio representing the tick period (i.e. the number
/// of second's fractions per tick)
/// @param offset offset value
/// @return time_point (system_clock)
template <typename req_tt = long long, typename period_tt = std::milli>
static constexpr inline clock::time_point
current_time_point(std::chrono::duration<req_tt, period_tt> offset) noexcept {
  return clock::now() + offset;
}

/// @brief current(now) timestamp
/// @tparam duration_tt duration type
/// @return timestamp of duration's ratio
template <typename duration_tt = default_duration>
static constexpr inline timestamp current_timestamp() noexcept {
  return std::chrono::duration_cast<duration_tt>(
             current_time_point<duration_tt>().time_since_epoch())
      .count();
}

#define MESSAGE(...)                                                           \
  do {                                                                         \
    std::cout << __VA_ARGS__ << std::endl;                                     \
  } while (0)

}; // namespace bt
