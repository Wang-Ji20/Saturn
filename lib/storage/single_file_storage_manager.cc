//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/single_file_storage_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/single_file_storage_manager.hh"
#include "catalog/database.hh"
#include "storage/single_file_block_manager.hh"

namespace saturn {

SingleFileStorageManager::SingleFileStorageManager(Database &database,
                                                   string path)
    : StorageManager(database, std::move(path)) {
  LoadDatabase();
}

void SingleFileStorageManager::LoadDatabase() {
  if (isInMemory()) {
    // TODO
    return;
  }
  auto &fileSys = database_.GetFS();

  // new database
  blockManager_ = make_unique<SingleFileBlockManager>(database_, path_);
  if (!fileSys.ExistFile(path_)) {
    // TODO
    // load exist database
    blockManager_->CreateNewDatabase();
  } else {
    blockManager_->LoadDatabase();
  }
  // TODO: start WAL
}

} // namespace saturn
