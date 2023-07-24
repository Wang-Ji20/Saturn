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
  // compute the checksum and write it to the start of the buffer (if not temp
  // buffer)
  uint64_t checksum = HashingUtilities::Checksum(block.buffer, block.limitSize);
  std::memcpy(block.buffer, &checksum, sizeof(checksum));
  // now write the buffer
  block.Write(*file_, location);
}

void SingleFileBlockManager::CreateNewDatabase() {
  OpenFlags flag = GetFileFlag(true);
  auto &fileSys = database_.GetFS();
  file_ = fileSys.Open(path_, flag);

  // write metadata!
  metadataBuffer_.Clear();
  CemeteryOfInnocent mainHeader;
  mainHeader.version = Storage::VERSION;
}

} // namespace saturn
