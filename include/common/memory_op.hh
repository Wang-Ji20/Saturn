//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/memory_op.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/type.hh"
#include <cstring>

namespace saturn {

struct MemoryUtils {

template <typename T>
static auto Load(const Datum *src, Offset location) -> T {
  T ret;
  std::memcpy(&ret, src + location, sizeof(T));
  return ret;
}

template <typename T>
static void Store(Datum *dst, Offset location, const T &value) {
  std::memcpy(dst + location, &value, sizeof(T));
}

};

} // namespace saturn
