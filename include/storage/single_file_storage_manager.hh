//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/single_file_storage_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "storage/block_manager.hh"
#include "storage/storage_manager.hh"

#include "common/unique_ptr.hh"

namespace saturn {

class SingleFileStorageManager : public StorageManager {
public:
  explicit SingleFileStorageManager(Database &database, string path);

protected:
  void LoadDatabase() override;

private:
  unique_ptr<BlockManager> blockManager_;
};

} // namespace saturn
