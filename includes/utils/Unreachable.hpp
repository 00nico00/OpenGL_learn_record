#pragma once

inline void unreachable() {
#if defined(_MSC_VER)
  __assume(false);
#elif defined(__clang__) || defined(__GNUC__)
  __builtin_unreachable();
#else
  static_assert(false, "unreachable is not impl on this platform");
#endif
}