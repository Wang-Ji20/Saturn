//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/block_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/block_manager.hh"
#include "common/mutex.hh"
#include "storage/block_handle.hh"

namespace saturn {

auto BlockManager::RegisterBlock(BlockId blockId, bool isMetaBlock)
    -> shared_ptr<BlockHandle> {
  scoped_lock<mutex> lock(blockLock_);
  auto entry = blockMap_.find(blockId);
  if (entry != blockMap_.end()) {
    auto existingBlock = entry->second.lock();
    if (existingBlock) {
      return existingBlock;
    }
  }
  // create new block
  auto newBlock = make_shared<BlockHandle>(*this, blockId);
  if (isMetaBlock) {
    metaBlockMap_[blockId] = newBlock;
  }
  blockMap_[blockId] = newBlock;
  return newBlock;
}

void BlockManager::UnregisterBlock(BlockId blockId) {
  scoped_lock<mutex> lock(blockLock_);
  blockMap_.erase(blockId);
}

} // namespace saturn
