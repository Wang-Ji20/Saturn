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
  Block(Allocator &allocator,
        BlockId id,
        Size internalSize = Storage::BLOCK_ALLOC_SIZE)
      : FileBuffer{allocator, FileBufferType::BLOCK, internalSize}, id{id} {};
  Block(Block &source, BlockId id)
      : FileBuffer{source, FileBufferType::BLOCK}, id{id} {}

public:
  BlockId id;
};

class BlockPointer {
public:
  BlockPointer(BlockId id, Offset offset) : id(id), offset(offset) {}
  BlockPointer() = default;

public:
  BlockId id;
  Offset offset;
};

} // namespace saturn
