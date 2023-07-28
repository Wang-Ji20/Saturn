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
#include "container/set.hh"
#include "port/file_system.hh"
#include "storage/block_manager.hh"

namespace saturn {

class Database;

struct StorageManagerOptions {
  bool read_only = false;
  bool use_direct_io = false;
};

class SingleFileBlockManager : public BlockManager {
public:
  ~SingleFileBlockManager() override = default;
  SingleFileBlockManager(Database &database,
                         string path,
                         StorageManagerOptions options = {});

  // essential methods
  DISALLOW_COPY(SingleFileBlockManager);
  SingleFileBlockManager(SingleFileBlockManager &&) noexcept;
  auto operator=(SingleFileBlockManager &&) noexcept
      -> SingleFileBlockManager &;

  // convert another kind of file buffer to block
  auto ConvertBlock(BlockId blockId, unique_ptr<FileBuffer> source)
      -> unique_ptr<Block> override;

  // create a new block, can reuse old file buffer.
  // source can be nullptr, we allocate new memory at that time.
  auto LoadCreateBlock(BlockId blockId, unique_ptr<FileBuffer> source)
      -> unique_ptr<Block> override;

  auto GetFreeBlockId() -> BlockId override;
  auto IsRootBlock(BlockId blockId) -> bool override;
  void MarkBlockAsFree(BlockId blockId) override;
  void MarkBlockAsModified(BlockId blockId) override;
  auto GetMetaBlock() -> BlockId override;
  void Read(Block &block) override;
  void Write(FileBuffer &block, BlockId blockId) override;

  auto CountBlocks() -> MemoryByte override;
  auto CountFreeBlocks() -> MemoryByte override;

  auto RegisterBlock(BlockId blockId, bool isMetaBlock = false)
      -> shared_ptr<BlockHandle>;
  void UnregisterBlock(BlockId blockId, bool canDestroy);

  void CreateNewDatabase();
  void LoadDatabase();

private:
  auto GetFileFlag(bool createNew = false) const -> OpenFlags;
  void ChecksumAndWrite(FileBuffer &block, MemoryByte location) const;
  void ReadAndChecksum(FileBuffer &block, MemoryByte location) const;

  // variables about file I/O
  StorageManagerOptions options_;
  Database &database_;
  string path_;
  unique_ptr<FileHandle> file_;
  FileBuffer metadataBuffer_;

  // guard
  mutable mutex mutex_;

  // variables for block management
  // free block list. we use them for now.
  set<BlockId> freeBlocks_;
  set<BlockId> modifiedBlocks_;
  u64 maxBlock;
  BlockId metaBlockId;
  BlockId freeListId;

  // variables for checkpoint
  u8 activeHeader;
  u64 iterativeCount;
};

} // namespace saturn
