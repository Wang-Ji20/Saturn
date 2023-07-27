//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/strong_typedef.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// typedefs that disallow implicit conversions
//         * scalar types ONLY *
// should have not much runtime overhead, according to
// Compiler Explorer (https://godbolt.org) at least in -O2
//===------------------------------------------------===

#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

#define STRONG_TYPEDEF_TYPE_CLASS(typeclass) STRONG_TYPEDEF_##typeclass

// 1. forbid implicit conversions from T
// 2. allow implicit conversions to T
#define STRONG_TYPEDEF_base(T, D)                                              \
  namespace saturn {                                                           \
  struct D {                                                                   \
    T t;                                                                       \
    explicit constexpr D(const T type_) : t(type_){};                          \
    D() = default;                                                             \
    D(const D &type_) = default;                                               \
    constexpr operator const T &() const { return t; }                         \
    constexpr operator T &() { return t; }                                     \
  };                                                                           \
  }                                                                            \
  namespace std {                                                              \
  template <> struct std::hash<saturn::D> {                                    \
    auto operator()(saturn::D const &ddd) const noexcept -> std::size_t {      \
      return std::hash<saturn::T>{}(ddd.t);                                    \
    }                                                                          \
  };                                                                           \
  }

#define STRONG_TYPEDEF_eq(T, D)                                                \
  namespace saturn {                                                           \
  struct D {                                                                   \
    T t;                                                                       \
    explicit constexpr D(const T type_) : t(type_){};                          \
    D() = default;                                                             \
    D(const D &type_) = default;                                               \
    constexpr operator const T &() const { return t; }                         \
    constexpr operator T &() { return t; }                                     \
    auto operator==(const D &rhs) const -> bool { return t == rhs.t; }         \
  };                                                                           \
  }                                                                            \
  namespace std {                                                              \
  template <> struct std::hash<saturn::D> {                                    \
    auto operator()(saturn::D const &ddd) const noexcept -> std::size_t {      \
      return std::hash<saturn::T>{}(ddd.t);                                    \
    }                                                                          \
  };                                                                           \
  }

#define STRONG_TYPEDEF_ord(T, D)                                               \
  namespace saturn {                                                           \
  struct D {                                                                   \
    T t;                                                                       \
    explicit constexpr D(const T type_) : t(type_){};                          \
    D() = default;                                                             \
    D(const D &type_) = default;                                               \
    constexpr operator const T &() const { return t; }                         \
    constexpr operator T &() { return t; }                                     \
    auto operator==(const D &rhs) const -> bool { return t == rhs.t; }         \
    auto operator<(const D &rhs) const -> bool { return t < rhs.t; }           \
  };                                                                           \
  }                                                                            \
  namespace std {                                                              \
  template <> struct std::hash<saturn::D> {                                    \
    auto operator()(saturn::D const &ddd) const noexcept -> std::size_t {      \
      return std::hash<saturn::T>{}(ddd.t);                                    \
    }                                                                          \
  };                                                                           \
  }

#define STRONG_TYPEDEF_num(T, D)                                               \
  namespace saturn {                                                           \
  struct D {                                                                   \
    T t;                                                                       \
    explicit constexpr D(const T type_) : t(type_){};                          \
    D() = default;                                                             \
    D(const D &type_) = default;                                               \
    constexpr operator const T &() const { return t; }                         \
    constexpr operator T &() { return t; }                                     \
    auto operator==(const D &rhs) const -> bool { return t == rhs.t; }         \
    auto operator<(const D &rhs) const -> bool { return t < rhs.t; }           \
    auto operator+(const D &rhs) const -> D { return D(t + rhs.t); }           \
    auto operator-(const D &rhs) const -> D { return D(t - rhs.t); }           \
    auto operator*(const D &rhs) const -> D { return D(t * rhs.t); }           \
    auto operator/(const D &rhs) const -> D { return D(t / rhs.t); }           \
  };                                                                           \
  constexpr auto operator""_##D(unsigned long long val) -> D {                 \
    return D(val);                                                             \
  }                                                                            \
  }                                                                            \
  namespace std {                                                              \
  template <> struct std::hash<saturn::D> {                                    \
    auto operator()(saturn::D const &ddd) const noexcept -> std::size_t {      \
      return std::hash<saturn::T>{}(ddd.t);                                    \
    }                                                                          \
  };                                                                           \
  }

/// strong typedef v2

namespace saturn {

template <typename Underlying> struct StrongTypedef_base {
  using UNDERLYING_TYPE = Underlying;
  Underlying t;
  explicit constexpr StrongTypedef_base(const Underlying type_) : t(type_){};
  StrongTypedef_base() = default;
  StrongTypedef_base(const StrongTypedef_base &type_) = default;
  operator const StrongTypedef_base &() const { return t; }
  operator StrongTypedef_base &() { return t; }
};

#if 0
// helper
template <typename T> struct SelectLastHelper {
  using type = T;
};

// template mixin
template <class... MIXINS>
struct StrongTypedef : public StrongTypedef_base<typename decltype((
                           SelectLastHelper<MIXINS>{}, ...))::type> {};

template <template <class CHILDREN> class MIXIN, class... MIXINS>
struct StrongTypedef<MIXIN<StrongTypedef<MIXINS...>>, MIXINS...>
    : public StrongTypedef<MIXINS...>,
      public MIXIN<StrongTypedef<MIXIN<StrongTypedef<MIXINS...>>, MIXINS...>> {
};

// additional functionalities are provided by typeclasses using mixins
template <class BASE> struct StrongTypedef_eq {
  auto operator==(const BASE &rhs) const -> bool { return BASE::t == rhs.t; }
};
#endif

template <class BASE> struct StrongTypedef_num {};

template <class BASE> struct StrongTypedef_ord {};

template <class BASE> struct StrongTypedef_show {};

} // namespace saturn

#define STRONG_TYPEDEF_V2_base(T, D)                                           \
  struct T : public saturn::StrongTypedef_base<D> {}
