//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\block.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===-------------------------------------------------------------===
// block is the basic unit of persistent spaces.
//
// block is not neccearily stored in memory, it may be evicted to disk,
// but exists. we need to load it when needed.
//
//===-------------------------------------------------------------===

#pragma once

#include "common/constant.hh"
#include "storage/file_buffer.hh"

namespace saturn {

class Block : public FileBuffer {
public:
  // constant
  static constexpr BlockId kInvalidBlockId = BlockId(-1LL);

  Block(Allocator &allocator,
        BlockId blockId,
        MemoryByte internalSize = Storage::BLOCK_ALLOC_SIZE)
      : FileBuffer{allocator, FileBufferType::BLOCK, internalSize}, id{blockId} {};
  Block(FileBuffer &source, BlockId blockId)
      : FileBuffer{source, FileBufferType::BLOCK}, id{blockId} {}


  BlockId id;
};

class BlockPointer {
public:
  BlockPointer(BlockId blockId, MemoryByte offset) : id(blockId), offset(offset) {}
  BlockPointer() = default;

  BlockId id;
  MemoryByte offset;
};

} // namespace saturn
