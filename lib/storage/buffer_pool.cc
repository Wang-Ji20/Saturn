//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/buffer_pool.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/buffer_pool.hh"
#include "common/logger.hh"

namespace saturn {

void BufferPoolReservation::Resize(MemoryByte newSize) {
  auto result = pool.IncreaseUsedMemory(newSize - size);
  CHECK(result.ok()) << result.message();
  size = newSize;
}

void BufferPoolReservation::Merge(BufferPoolReservation &&other) {
  size += other.size;
  other.size = 0ULL;
}

auto BufferEvictionNode::CanUnload(BlockHandle &handle) -> bool {
  if (timestamp_ != handle.timestamp_.load()) {
    return false;
  }
  return handle.CanUnload();
}

auto BufferEvictionNode::TryGetBlockHandle() -> shared_ptr<BlockHandle> {
  auto handle = handle_.lock();
  if (handle == nullptr) {
    return nullptr;
  }
  if (!CanUnload(*handle)) {
    return nullptr;
  }
  return handle;
}

auto BufferPool::IncreaseUsedMemory(MemoryByte size) -> Status {
  // https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
  auto usedMemory = usedMemory_.load();
  while (!usedMemory_.compare_exchange_weak(usedMemory, usedMemory + size)) {
    if (usedMemory + size > maxMemory_.load()) {
      return absl::ResourceExhaustedError("Buffer pool is full.");
    }
  }
  return absl::OkStatus();
}

auto BufferPool::SetMaxMemory(MemoryByte size) -> Status {
  auto maxMemory = maxMemory_.load();
  while (!maxMemory_.compare_exchange_weak(maxMemory, size)) {
    if (maxMemory < usedMemory_.load()) {
      return absl::ResourceExhaustedError("Buffer pool is full.");
    }
  }
  return absl::OkStatus();
}

auto BufferPool::EvictBlocks(MemoryByte extraMemory, MemoryByte memoryLimit)
    -> Eviction {
  BufferEvictionNode node;
  BufferPoolReservation resMem{*this, extraMemory};
  while (usedMemory_.load() > memoryLimit) {
    if (evictionQueue_->TryDequeue(node)) {
      resMem.Resize(0ULL);
      return {false,
              std::move(resMem),
              absl::ResourceExhaustedError("no more memory")};
    }
    auto handle = node.TryGetBlockHandle();
    if (handle == nullptr) {
      continue;
    }
    scoped_lock<mutex> lock{handle->lock_};
    if (!node.CanUnload(*handle)) {
      continue;
    }
    if (handle->buffer_->GetAllocationSize() == extraMemory) {
      return {true, std::move(resMem), std::move(handle->buffer_)};
    }
    handle->Unload();
  }
  return {true, std::move(resMem), absl::NotFoundError("no exact size buffer")};
}

} // namespace saturn
