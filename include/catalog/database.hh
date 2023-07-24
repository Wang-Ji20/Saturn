//===------------------------------------------===
// Saturn 2023
//
// Identification: include/catalog/database.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/unique_ptr.hh"
#include "main/config.hh"

namespace saturn {

class StorageManager;
class BufferManager;
class Catalog;

class Database : std::enable_shared_from_this<Database> {
public:
  inline auto GetStorageManager() -> StorageManager & {
    return *storageManager_;
  }
  inline auto GetBufferManager() -> BufferManager & { return *bufferManager_; }
  inline auto GetCatalog() -> Catalog & { return *catalog_; }
  inline auto GetFS() const -> FileSystem & { return *config.fileSystem; }
  inline auto GetAllocator() const -> Allocator & { return *config.allocator; }

  // NOTE: we put replaceable components in DBConfig, user may change it using
  // .yaml files. Inreplaceable components are put in Database, user cannot
  // and should not change it.
  DBConfig config;

private:
  unique_ptr<StorageManager> storageManager_;
  unique_ptr<BufferManager> bufferManager_;
  unique_ptr<Catalog> catalog_;
};

} // namespace saturn
