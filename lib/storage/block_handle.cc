//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/block_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/block_handle.hh"
#include "storage/block_manager.hh"
#include "storage/buffer_handle.hh"
#include "storage/buffer_manager.hh"

namespace saturn {

BlockHandle::BlockHandle(BlockManager &blockManager, BlockId blockId)
    : blockManager_{blockManager},
      timestamp_{0_Size},
      status_{BlockStatus::UNLOADED},
      readers_{0},
      blockId_{blockId},
      memoryUsage_(Storage::BLOCK_ALLOC_SIZE),
      memoryCharge_{blockManager.bufferManager_.GetBufferPool()},
      buffer_{nullptr},
      canDestroy_{false} {}

// TODO
auto BlockHandle::Load(shared_ptr<BlockHandle> &handle,
                       unique_ptr<FileBuffer> buffer) -> BufferHandle {
  return BufferHandle{};
}

// TODO
auto BlockHandle::Unload() -> void {}

// TODO
auto BlockHandle::CanUnload() -> bool { return false; }

} // namespace saturn
