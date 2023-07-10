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
#include "common/result.hh"
#include "common/type.hh"
#include "common/macro.hh"
#include "container/concurrent_queue.hh"
#include "storage/file_buffer.hh"
#include "storage/block_handle.hh"

namespace saturn {

struct BufferEvictionNode {
  BufferEvictionNode() = default;
  BufferEvictionNode(weak_ptr<BlockHandle> handle, Size timestamp)
      : handle_{std::move(handle)}, timestamp_{timestamp} {}

  weak_ptr<BlockHandle> handle_;
  Size timestamp_;

  auto CanUnload(BlockHandle& handle) -> bool;
  auto TryGetBlockHandle() -> shared_ptr<BlockHandle>;
};

class BufferPool {
public:
  explicit BufferPool(Size maxSize) :
      maxMemory_{maxSize}
      {}
  DISALLOW_COPY(BufferPool);

  inline auto GetMaxMemory() -> Size { return Size(maxMemory_); }
  inline auto GetUsedMemory() -> Size { return Size(usedMemory_); }
  auto IncreaseUsedMemory(Size size) -> Status;
  auto SetMaxMemory(Size size) -> Status;

protected:
  struct Eviction {
    bool ok;
    BufferPoolReservation reservation;
    result<unique_ptr<FileBuffer>> reuseBuffer;
  };
  auto EvictBlocks(Offset extraMemory, Size memoryLimit) -> Eviction;

  // garbage cleaning routines.
  void GCQueue();
  void AddToEvictionQueue(shared_ptr<BlockHandle> &handle);

private:
  atomic<Size> maxMemory_;
  atomic<Size> usedMemory_ {0_Size};
  unique_ptr<ConcurrentQueue<BufferEvictionNode>> evictionQueue_;
  atomic<u32> queueInsertions_;
};



} // namespace saturn
