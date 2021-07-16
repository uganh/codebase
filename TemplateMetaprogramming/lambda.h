#pragma once

template <unsigned N> struct arg { constexpr static unsigned value = N; };

template <unsigned int N, typename Arg, typename... Params> struct index {
  using type = typename index<N - 1, Params...>::type;
};

template <typename Arg, typename... Params> struct index<1, Arg, Params...> {
  using type = Arg;
};

// For common type
template <typename Ty, typename... Args> struct lambda { using type = Ty; };

// Specification for placeholders
template <unsigned N, typename... Args> struct lambda<arg<N>, Args...> {
  using type = typename index<N, Args...>::type;
};

// Specification for nested templates
template <template <typename... Params> class Template, typename... Params,
          typename... Args>
struct lambda<Template<Params...>, Args...> {
  using type =
      typename lambda<Template<Params...>>::type::template apply<Args...>::type;
};

// Specification for metafunctions
template <template <typename... Params> class Template, typename... Params>
struct lambda<Template<Params...>> {
  // Return a metafunction class
  struct type {
    template <typename U, typename = void> struct wrapper { using type = U; };

    template <typename U> struct wrapper<U, typename U::type> {
      using type = typename U::type;
    };

    template <class... Args> struct apply {
      using type = typename wrapper<
          Template<typename lambda<Params, Args...>::type...>>::type;
    };
  };
};

template <typename F, typename... Args> struct apply {
  using type = typename lambda<F>::type::template apply<Args...>::type;
};

namespace placeholders {
using _1 = arg<1>;
using _2 = arg<2>;
using _3 = arg<3>;
using _4 = arg<4>;
using _5 = arg<5>;
} // namespace placeholders
