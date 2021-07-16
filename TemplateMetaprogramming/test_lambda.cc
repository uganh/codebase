#include <iostream>
#include <vector>

#include "lambda.h"

template <long N> struct integer { static constexpr long value = N; };

/*
 * Metafunctions
 */

template <typename X> struct Identify {
  static constexpr long value = X::value;
};

template <typename X, typename Y> struct Add {
  static constexpr long value = X::value + Y::value;
};

template <typename X, typename Y> struct Sub {
  static constexpr long value = X::value - Y::value;
};

template <typename X, typename Y> struct Mul {
  static constexpr long value = X::value * Y::value;
};

template <typename X, typename Y> struct Div {
  static constexpr long value = X::value / Y::value;
};

using namespace placeholders;

int main(void) {
  std::cout
      << lambda<Add<_1, _1>>::type::template apply<integer<25>>::type::value
      << std::endl;
  std::cout << apply<Add<_1, _2>, integer<25>, integer<30>>::type::value
            << std::endl;

  // (25 + 20) * (25 - 20)
  std::cout << apply<Mul<Add<_1, _2>, Sub<_1, _2>>, integer<25>,
                     integer<20>>::type::value
            << std::endl;

  static_assert(std::is_same<apply<std::vector<_1>, double>::type,
                             std::vector<double>>::value,
                "Unexpected");

  return 0;
}