//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/optional.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// this file wraps the optional type.
//===------------------------------------------------===

#pragma once

#include "absl/types/optional.h"

namespace saturn {
template <typename T> using optional = absl::optional<T>;
} // namespace saturn
