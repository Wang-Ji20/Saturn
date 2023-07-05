//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/align.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

namespace saturn {

template <typename T>
static inline constexpr auto 
AlignDown(T x, T y) -> T {
  return ((x + (y - T(1))) / y) * y;
}

template <typename T>
static inline constexpr auto
IsAlligned(T x, T y) -> bool {
  return (x % y) == T(0);
}

} // namespace saturn
