//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\macro.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===--------------------------------------===
// this file contains some common macros.
// NB. we only use macros when no alternatives.
//===--------------------------------------===

#pragma once

#undef DISALLOW_COPY
#define DISALLOW_COPY(classname)                                               \
  classname(const classname &) = delete;                                       \
  auto operator=(const classname &)->classname & = delete;                     \
  classname(classname &) = delete;                                             \
  auto operator=(classname &)->classname & = delete

#undef DISALLOW_MOVE
#define DISALLOW_MOVE(classname)                                               \
  classname(classname &&) = delete;                                            \
  auto operator=(classname &&)->classname && = delete

#undef DISALLOW_COPY_AND_MOVE
#define DISALLOW_COPY_AND_MOVE(classname)                                      \
  DISALLOW_COPY(classname);                                                    \
  DISALLOW_MOVE(classname)

#undef CAST_TO_DERIVED
#define CAST_TO_DERIVED(classname)                                             \
  template <class T> auto Cast()->T & {                                        \
    static_assert(std::is_base_of_v<classname, T> == true, "invalid cast");    \
    return reinterpret_cast<T &>(*this);                                       \
  }                                                                            \
  template <class T> auto Cast() const->const T & {                            \
    static_assert(std::is_base_of_v<classname, const T> == true,               \
                  "invalid cast");                                             \
    return reinterpret_cast<const T &>(*this);                                 \
  }

//===-------------------------------------------------------------===
// This is an enum class with:
// * | and & operator
// * ContainFlags function test if a flag is contained in a flag set
//===-------------------------------------------------------------===

#undef SATURN_ENUM_CLASS
#define SATURN_ENUM_CLASS(enumName, underlyingType)                            \
  enum class enumName : underlyingType;                                        \
  inline constexpr auto operator|(enumName lhs, enumName rhs)->enumName {      \
    return static_cast<enumName>(static_cast<underlyingType>(lhs) |            \
                                 static_cast<underlyingType>(rhs));            \
  }                                                                            \
  inline constexpr auto operator|=(enumName &lhs, enumName rhs)->enumName & {  \
    lhs = lhs | rhs;                                                           \
    return lhs;                                                                \
  }                                                                            \
  inline constexpr auto operator&(enumName lhs, enumName rhs)->enumName {      \
    return static_cast<enumName>(static_cast<underlyingType>(lhs) &            \
                                 static_cast<underlyingType>(rhs));            \
  }                                                                            \
  inline constexpr auto operator&=(enumName &lhs, enumName rhs)->enumName & {  \
    lhs = lhs & rhs;                                                           \
    return lhs;                                                                \
  }                                                                            \
  inline constexpr auto operator~(enumName flags)->enumName {                  \
    return static_cast<enumName>(~static_cast<underlyingType>(flags));         \
  }                                                                            \
  inline constexpr auto ContainFlags(enumName flags, enumName test_flag)       \
      ->bool {                                                                 \
    return (flags & test_flag) == test_flag;                                   \
  }                                                                            \
  template <class enumName, typename... Args>                                  \
  inline constexpr auto ContainFlags(                                          \
      enumName flags, enumName test_flag, Args... args)                        \
      ->bool {                                                                 \
    return ((flags & test_flag) == test_flag) || ContainFlags(flags, args...); \
  }                                                                            \
  inline constexpr auto ContainFlagsAnd(enumName flags, enumName test_flag)    \
      ->bool {                                                                 \
    return (flags & test_flag) == test_flag;                                   \
  }                                                                            \
  template <class enumName, typename... Args>                                  \
  inline constexpr auto ContainFlagsAnd(                                       \
      enumName flags, enumName test_flag, Args... args)                        \
      ->bool {                                                                 \
    return ((flags & test_flag) == test_flag) && ContainFlags(flags, args...); \
  }                                                                            \
  enum class enumName : underlyingType
