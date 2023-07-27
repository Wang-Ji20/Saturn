//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/default_buffer_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/default_buffer_manager.hh"

namespace saturn {

DefaultBufferManager::DefaultBufferManager(Database &database)
    : database_(database) {}

} // namespace saturn
