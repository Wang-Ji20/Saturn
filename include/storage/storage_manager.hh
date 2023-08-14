//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/storage_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// this file manages the disk. performs WAL
//===------------------------------------------------===

#pragma once

#include <utility>

#include "common/macro.hh"
#include "common/string.hh"

namespace saturn {

class Database;

class StorageManager {
public:
  StorageManager(Database &database, string path);
  ~StorageManager() = default;

  CAST_TO_DERIVED(StorageManager);
  auto isInMemory() -> bool;

protected:
  virtual void LoadDatabase() = 0;

  Database &database_;
  string path_;
};

} // namespace saturn
