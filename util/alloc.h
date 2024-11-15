#pragma once
#include <cstdint>
#include <memory>
#include <stack>
#include <vector>

namespace util {

static constexpr int32_t size_unlimited = -1;

template <class object_tt, int32_t size_vv>
class allocator {
 public:
  using object_type = std::decay_t<object_tt>;
  using object_ptr = std::unique_ptr<object_type>;

 protected:
  std::vector<object_ptr> _alloceds;

 private:
  object_ptr __allocate() {
    object_type* result;
    if (!_alloceds.empty()) {
      result = _alloceds.back().release();
      _alloceds.pop_back();
    } else {
      result = static_cast<object_ptr>(::operator new(sizeof(object_tt)));
    }
    return result;
  }

 public:
  allocator() { _alloceds.reserve(size_vv); }

  virtual ~allocator() { _alloceds.clear(); }

  template <typename... Args>
  object_ptr allocate(Args &&...args) {
    object_ptr place = __allocate();
    try {
      new (place) object_tt(std::forward<Args>(args)...);
    } catch (...) {
      _alloceds.push_back(place);
      throw;
    }
    return place;
  }

  void deallocate(object_ptr obj) {
    obj->~object_tt();

    if (_alloceds.size() >= size_vv) {
      ::operator delete(obj);
      obj = nullptr;
    } else {
      _alloceds.push_back(obj);
    }
  }
};

template <class object_tt>
class allocator<object_tt, size_unlimited> {
 public:
  using object_type = std::decay_t<object_tt>;
  using object_ptr = std::unique_ptr<object_type>;

 protected:
  std::stack<object_ptr> _alloceds;

 private:
  void *__allocate() {
    object_type *result;
    if (!_alloceds.empty()) {
      result = _alloceds.top().release();
      _alloceds.pop();
    } else {
      result = static_cast<object_ptr>(::operator new(sizeof(object_tt)));
    }
    return result;
  }

 public:
  allocator() {}

  virtual ~allocator() { _alloceds.clear(); }

  template <typename... Args>
  object_ptr allocate(Args &&...args) {
    object_ptr place = __allocate();
    try {
      new (place) object_tt(std::forward<Args>(args)...);
    } catch (...) {
      _alloceds.push(place);
      throw;
    }
    return place;
  }

  void deallocate(object_ptr obj) {
    obj->~object_tt();
    _alloceds.push(obj);
  }
};
}  // namespace util

/*
 *
    {
        easy::alloc::allocator<timer_cost, -1> unlimited_alloc;

        auto ptr = std::shared_ptr<timer_cost>(unlimited_alloc.allocate(),
            [&unlimited_alloc](auto* ptr) { unlimited_alloc.deallocate(ptr); });

    }
    {
        easy::alloc::allocator<timer_cost, -1> ulimited_alloc;
        easy::alloc::allocator<timer_cost, 5> limited_alloc;

        std::set<timer_cost*> allocs;
        for (int i = 0; i < 100; ++i) {
            auto one = ulimited_alloc.allocate();

            if (i % 5 == 0) {
                ulimited_alloc.deallocate(one);
                continue;
            }

            allocs.emplace(one);
        }

        for (auto one : allocs) {
            ulimited_alloc.deallocate(one);
        }
        allocs.clear();

        for (int i = 0; i < 100; ++i) {
            auto one = limited_alloc.allocate();

            if (i % 5 == 0) {
                limited_alloc.deallocate(one);
                continue;
            }

            allocs.emplace(one);
        }

        for (auto one : allocs) {
            limited_alloc.deallocate(one);
        }
    }
 */
