#pragma once

#include <concepts>
#include <functional>

template <typename Func, typename... Args>
  requires std::invocable<Func, Args...>
struct Guard {
  Guard(Func&& func, Args&&... args) {
    inner = [func = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
      std::invoke(func, std::move(args)...);
    };
  }

  ~Guard() {
    if (inner) {
      inner();
    }
  }

  Guard(const Guard&) = delete;
  Guard& operator=(const Guard&) = delete;
  Guard(Guard&&) = delete;
  Guard& operator=(Guard&&) = delete;

private:
  std::function<void()> inner{};
};
