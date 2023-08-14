//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/storage_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/storage_manager.hh"

namespace saturn {

StorageManager::StorageManager(Database &database, string path = ":memory:")
    : database_(database), path_{std::move(path)} {}

auto StorageManager::isInMemory() -> bool { return path_ == ":memory:"; }

} // namespace saturn
