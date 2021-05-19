#pragma once

#include <cassert>
#include <utility>

#define __scopeguard_concat_impl(name, line) name##line
#define __scopeguard_concat(name, line) __scopeguard_concat_impl(name, line)

template <typename Function> class ScopeGuard {
  Function F;

public:
  ScopeGuard(void) = delete;

  ScopeGuard(Function &&F) : F(std::forward<Function>(F)) {}

  ScopeGuard(const ScopeGuard &) = delete;
  ScopeGuard &operator=(const ScopeGuard &) = delete;

  ScopeGuard(ScopeGuard &&rhs) : F(std::move(rhs.F)) { assert(false); }

  ~ScopeGuard(void) { F(); }
};

enum class ScopeGuardHelper {};

template <typename Function>
inline ScopeGuard<Function> operator+(ScopeGuardHelper, Function &&F) {
  /* WARNING: May deconstruct twice */
  return ScopeGuard<Function>(std::forward<Function>(F));
}

#define defer                                                                  \
  auto __scopeguard_concat(__defer__, __LINE__) = ScopeGuardHelper() + [&]
