//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/single_file_block_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/constant.hh"
#include "storage/block_manager.hh"

namespace saturn {

class SingleFileBlockManager : public BlockManager {
public:
  ~SingleFileBlockManager() override = default;
  explicit SingleFileBlockManager(BufferManager &bufferManager)
      : BlockManager(bufferManager) {}

  // essential methods
  DISALLOW_COPY(SingleFileBlockManager);
  SingleFileBlockManager(SingleFileBlockManager &&) noexcept;
  auto operator=(SingleFileBlockManager &&) noexcept
      -> SingleFileBlockManager &;

  // convert another kind of file buffer to block
  // no ownership transfer, just borrowing (because block don't neccesarilly in
  // memory)
  auto ConvertBlock(BlockId blockId, FileBuffer &source)
      -> unique_ptr<Block> override;

  // create a new block, can reuse old file buffer.
  // source can be nullptr, we allocate new memory at that time.
  // so use raw pointer here.
  auto CreateBlock(BlockId blockId, FileBuffer *source)
      -> unique_ptr<Block> override;

  auto GetFreeBlockId() -> BlockId override;
  auto IsRootBlock(BlockId blockId) -> bool override;
  void MarkBlockAsFree(BlockId blockId) override;
  auto MarkBlockAsModified(BlockId blockId) -> bool override;
  void IncreaseBlockReferenceCount(BlockId blockId) override;
  auto GetMetaBlock() -> BlockId override;
  void Read(Block &block) override;
  void Write(FileBuffer &block, BlockId blockId) override;

  auto CountBlocks() -> Size override;
  auto CountFreeBlocks() -> Size override;

  auto RegisterBlock(BlockId blockId, bool isMetaBlock = false)
      -> shared_ptr<BlockHandle>;
  void UnregisterBlock(BlockId blockId, bool canDestroy);
};

} // namespace saturn
