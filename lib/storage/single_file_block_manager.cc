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
#include "storage/single_file_block_manager.hh"

namespace saturn {

SingleFileBlockManager::SingleFileBlockManager(Database &database, string path)
    : BlockManager(database.GetBufferManager()),
      database_(database),
      path_(std::move(path)) {}

} // namespace saturn
