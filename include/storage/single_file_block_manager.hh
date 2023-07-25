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

  void CreateNewDatabase();
  void LoadDatabase();

private:
  auto GetFileFlag(bool createNew = false) const -> OpenFlags;
  void ChecksumAndWrite(FileBuffer &block, Offset location) const;
  void ReadAndChecksum(FileBuffer &block, Offset location) const;

  StorageManagerOptions options_;
  Database &database_;
  string path_;
  unique_ptr<FileHandle> file_;
  FileBuffer metadataBuffer_;
  mutable mutex mutex_;

  u8 activeHeader;
  u64 iterativeCount;
  u64 maxBlock;
};

} // namespace saturn
