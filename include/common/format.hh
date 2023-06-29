//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\format.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "absl/strings/str_format.h"

namespace saturn {
template <typename... Args> auto format(Args &&...args) -> std::string {
    return absl::StrFormat(std::forward<Args>(args)...);
}
} // namespace saturn
