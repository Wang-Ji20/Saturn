//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/constant.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/type.hh"

namespace saturn {

struct Storage {
  constexpr static Size SECTOR_SIZE = 4096_Size;
  constexpr static Size BLOCK_HEADER_SIZE = Size(sizeof(u64));
  constexpr static Size BLOCK_ALLOC_SIZE = 262144_Size;
};

} // namespace saturn