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
      timestamp_{0ULL},
      status_{BlockStatus::UNLOADED},
      readers_{0},
      blockId_{blockId},
      memoryUsage_(Storage::BLOCK_ALLOC_SIZE),
      memoryCharge_{blockManager.bufferManager_.GetBufferPool()},
      buffer_{nullptr} {}

BlockHandle::~BlockHandle() {
  if (status_ == BlockStatus::LOADED) {
    memoryCharge_.Resize(0ULL);
  }
  blockManager_.UnregisterBlock(blockId_);
}

auto BlockHandle::Load(shared_ptr<BlockHandle> &handle,
                       unique_ptr<FileBuffer> buffer) -> BufferHandle {
  if (handle->status_ == BlockStatus::LOADED) {
    DCHECK(handle->buffer_ != nullptr);
    return {handle, handle->buffer_.get()};
  }

  auto &blockManager = handle->blockManager_;
  // invalid blockId, deal with it later
  if (handle->blockId_ == BlockManager::kMaxBlockId) {
    return BufferHandle{};
  }
  auto block =
      blockManager.LoadCreateBlock(handle->blockId_, std::move(buffer));
  blockManager.Read(*block);
  handle->status_ = BlockStatus::LOADED;
  handle->buffer_ = std::move(block);
  return {handle, handle->buffer_.get()};
}

auto BlockHandle::Unload() -> unique_ptr<FileBuffer> {
  if (status_ == BlockStatus::UNLOADED) {
    return nullptr;
  }
  DCHECK(CanUnload()) << "cannot unload this block.";
  memoryCharge_.Resize(0ULL);
  status_ = BlockStatus::UNLOADED;
  return std::move(buffer_);
}

auto BlockHandle::CanUnload() -> bool {
  // already unloaded, cannot unload more
  if (status_ == BlockStatus::UNLOADED) {
    return false;
  }
  // someone reading, cannot unload
  if (readers_ > 0) {
    return false;
  }
  return true;
}

} // namespace saturn
