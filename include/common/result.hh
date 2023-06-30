//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\result.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "absl/status/statusor.h"

namespace saturn {
template <typename T> using result = absl::StatusOr<T>;
} // namespace saturn
