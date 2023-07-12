//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\block_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// provides a unified interface for all block managers.
//===----------------------------------------------------===

#pragma once

#include "common/macro.hh"
#include "common/mutex.hh"
#include "common/type.hh"

#include "container/unordered_map.hh"

#include "storage/block.hh"

namespace saturn {

class BlockHandle;
class BufferManager;

class BlockManager {
public:
  virtual ~BlockManager() = default;
  explicit BlockManager(BufferManager &bufferManager)
      : bufferManager_(bufferManager) {}

  BufferManager &bufferManager_;
  // essential methods
  DISALLOW_COPY_AND_MOVE(BlockManager);

  // convert another kind of file buffer to block
  // no ownership transfer, just borrowing (because block don't neccesarilly in memory)
  virtual auto ConvertBlock(BlockId blockId, FileBuffer &source) -> unique_ptr<Block> = 0;

  // create a new block, can reuse old file buffer.
  // source can be nullptr, we allocate new memory at that time.
  // so use raw pointer here.
  virtual auto CreateBlock(BlockId blockId, FileBuffer *source) -> unique_ptr<Block> = 0;

  virtual auto GetFreeBlockId() -> BlockId = 0;
  virtual auto IsRootBlock(BlockId blockId) -> bool = 0;
  virtual void MarkBlockAsFree(BlockId blockId) = 0;
  virtual auto MarkBlockAsModified(BlockId blockId) -> bool = 0;
  virtual void IncreaseBlockReferenceCount(BlockId blockId) = 0;
  virtual auto GetMetaBlock() -> BlockId = 0;
  virtual void Read(Block &block) = 0;
  virtual void Write(FileBuffer &block, BlockId blockId) = 0;
  virtual void Write(Block &block) { Write(block, block.id); };

  virtual auto CountBlocks() -> Size = 0;
  virtual auto CountFreeBlocks() -> Size = 0;

  auto RegisterBlock(BlockId blockId, bool isMetaBlock = false) -> shared_ptr<BlockHandle>;
  void UnregisterBlock(BlockId blockId, bool canDestroy);

private:
  mutable mutex blockLock_;
  unordered_map<BlockId, weak_ptr<BlockHandle>> blockMap_;
  unordered_map<BlockId, shared_ptr<BlockHandle>> metaBlockMap_;
};

} // namespace saturn
