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
AlignDown(T source, T alignBy) -> T {
  return ((source + (alignBy - T(1))) / alignBy) * alignBy;
}

template <typename T>
static inline constexpr auto
IsAlligned(T souce, T alignBy) -> bool {
  return (souce % alignBy) == T(0);
}

} // namespace saturn
