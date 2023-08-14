//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\buffer_pool.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// this file contains the buffer pool in memory.
//===----------------------------------------------------===

#pragma once

#include "common/atomic.hh"
#include "common/macro.hh"
#include "common/result.hh"
#include "common/type.hh"
#include "container/concurrent_queue.hh"
#include "storage/block_handle.hh"
#include "storage/file_buffer.hh"

namespace saturn {

struct BufferEvictionNode {
  BufferEvictionNode() = default;
  BufferEvictionNode(weak_ptr<BlockHandle> handle, MemoryByte timestamp)
      : handle_{std::move(handle)}, timestamp_{timestamp} {}

  weak_ptr<BlockHandle> handle_;
  MemoryByte timestamp_;

  auto CanUnload(BlockHandle &handle) const -> bool;
  [[nodiscard]] auto TryGetBlockHandle() const -> shared_ptr<BlockHandle>;
};

class BufferPool {
  friend class DefaultBufferManager;
  friend struct BufferPoolReservation;

public:
  explicit BufferPool(MemoryByte maxSize) : maxMemory_{maxSize} {}
  DISALLOW_COPY(BufferPool);

  inline auto GetMaxMemory() -> MemoryByte { return maxMemory_; }
  inline auto GetUsedMemory() -> MemoryByte { return usedMemory_; }
  auto IncreaseUsedMemory(MemoryByte size) -> Status;
  auto SetMaxMemory(MemoryByte size) -> Status;

protected:
  struct Eviction {
    bool ok;
    BufferPoolReservation reservation;
    result<unique_ptr<FileBuffer>> reuseBuffer;
  };
  auto EvictBlocks(MemoryByte extraMemory, MemoryByte memoryLimit) -> Eviction;

  // garbage cleaning routines.
  void GCQueue();
  void AddToEvictionQueue(shared_ptr<BlockHandle> &handle);

private:
  atomic<MemoryByte> maxMemory_;
  atomic<MemoryByte> usedMemory_{0ULL};
  unique_ptr<ConcurrentQueue<BufferEvictionNode>> evictionQueue_;
  atomic<u32> queueInsertions_;
};

} // namespace saturn
