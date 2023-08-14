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
  constexpr static MemoryByte SECTOR_SIZE = 4096;
  constexpr static MemoryByte BLOCK_HEADER_SIZE = sizeof(u64);
  constexpr static MemoryByte BLOCK_ALLOC_SIZE = 262144;
  constexpr static Version VERSION = Version(0ULL);
  constexpr static MemoryByte BLOCK_SECTION_BEGIN = SECTOR_SIZE * 3;

  constexpr static auto GetBlockOffset(BlockId blockId) -> MemoryByte {
    return BLOCK_SECTION_BEGIN + static_cast<u64>(blockId) * BLOCK_ALLOC_SIZE;
  }
};

} // namespace saturn
