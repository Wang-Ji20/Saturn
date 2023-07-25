//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/single_file_block_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include <utility>

#include "catalog/database.hh"
#include "common/checksum.hh"
#include "common/memory_op.hh"
#include "common/serializer/binary_serializer.hh"
#include "storage/single_file_block_manager.hh"
#include "storage/storage_magic.hh"

namespace saturn {

SingleFileBlockManager::SingleFileBlockManager(Database &database,
                                               string path,
                                               StorageManagerOptions options)
    : BlockManager(database.GetBufferManager()),
      options_(options),
      database_(database),
      path_(std::move(path)),
      metadataBuffer_(database_.GetAllocator(),
                      FileBufferType::MANAGED_BUFFER,
                      Storage::SECTOR_SIZE - Storage::BLOCK_HEADER_SIZE) {}

auto SingleFileBlockManager::GetFileFlag(bool createNew) const -> OpenFlags {
  // at least we can read
  OpenFlags flag = OpenFlags::READ;
  if (createNew) {
    flag |= OpenFlags::CREATE;
  }
  if (options_.use_direct_io) {
    flag |= OpenFlags::DIRECT_IO;
  }
  if (!options_.read_only) {
    flag |= OpenFlags::WRITE;
  }
  return flag;
}

void SingleFileBlockManager::ChecksumAndWrite(FileBuffer &block,
                                              Offset location) const {
  // compute the checksum and write it to the start of the buffer
  uint64_t checksum = HashingUtilities::Checksum(block.buffer, block.limitSize);
  std::memcpy(block.buffer, &checksum, sizeof(checksum));
  // now write the buffer
  block.Write(*file_, location);
}

void SingleFileBlockManager::ReadAndChecksum(FileBuffer &block,
                                             Offset location) const {
  block.Read(*file_, location);
  auto fileChecksum = MemoryUtils::Load<u64>(block.internalBuffer, 0_Offset);
  auto checksum = HashingUtilities::Checksum(block.buffer, block.limitSize);
  if (fileChecksum != checksum) {
    throw IllegalArgumentException("trying to read a corrupted block.");
  }
}

void SingleFileBlockManager::CreateNewDatabase() {
  OpenFlags flag = GetFileFlag(true);
  auto &fileSys = database_.GetFS();
  file_ = fileSys.Open(path_, flag);

  // write metadata!
  metadataBuffer_.Clear();
  CemeteryOfInnocent mainHeader;
  mainHeader.version = Storage::VERSION;
  auto serializerResult = BinarySerializer::Serialize(
      mainHeader, metadataBuffer_.buffer, Size(sizeof(mainHeader)));
  if (!serializerResult.ok()) {
    throw InternalException("failed to serialize file header");
  }
  ChecksumAndWrite(metadataBuffer_, 0_Offset);

  // write metadata about database.
  // double buffer headers
  metadataBuffer_.Clear();
  DatabaseHeader databaseHeader = {
      .iteration = 0,
      .rootMeta = Block::kInvalidBlockId,
      .freeList = Block::kInvalidBlockId,
      .blockCount = 0,
  };
  serializerResult = BinarySerializer::Serialize(
      databaseHeader, metadataBuffer_.buffer, Size(sizeof(databaseHeader)));
  if (!serializerResult.ok()) {
    throw InternalException("failed to serialize database header");
  }
  for (int i = 1; i < 3; ++i) {
    ChecksumAndWrite(metadataBuffer_,
                     Offset(i * static_cast<i64>(Storage::SECTOR_SIZE)));
  }
  file_->Sync();
  activeHeader = 1;
  iterativeCount = 0;
  maxBlock = 0;
}

auto SingleFileBlockManager::ConvertBlock(BlockId blockId,
                                          unique_ptr<FileBuffer> source)
    -> unique_ptr<Block> {
  DCHECK(source->internalSize == Storage::BLOCK_ALLOC_SIZE);
  return make_unique<Block>(*source, blockId);
}

auto SingleFileBlockManager::LoadCreateBlock(BlockId blockId,
                                             unique_ptr<FileBuffer> source)
    -> unique_ptr<Block> {
  // reusable
  if (source != nullptr) {
    return ConvertBlock(blockId, std::move(source));
  }
  // new block
  return make_unique<Block>(database_.GetAllocator(), blockId);
}

} // namespace saturn
