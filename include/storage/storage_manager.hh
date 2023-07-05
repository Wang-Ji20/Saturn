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

class StorageManager {
public:
  StorageManager(string path) : path_{std::move(path)} {};

  CAST_TO_DERIVED(StorageManager);

private:
  string path_;
};

} // namespace saturn
