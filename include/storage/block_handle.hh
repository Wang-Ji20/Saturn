//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/block_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/type.hh"

namespace saturn {

enum class BlockStatus : u8 {
  LOADED,
  UNLOADED,
};

class BlockHandle {
};

} // namespace saturn
