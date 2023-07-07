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

class BufferPool;

struct BufferPoolReservation {
  Size size {0};
  BufferPool& pool;

  explicit BufferPoolReservation(BufferPool& bufferPool): pool {bufferPool} {}
  BufferPoolReservation(BufferPool& bufferPool, Size size) : pool {bufferPool} {
    Resize(size);
  }
  DISALLOW_COPY(BufferPoolReservation);
  BufferPoolReservation(BufferPoolReservation&& other) noexcept :
  pool {other.pool} {
    size = other.size;
    other.size = 0_Size;
  }
  auto operator=(BufferPoolReservation&& other) noexcept -> BufferPoolReservation& {
    size = other.size;
    other.size = 0_Size;
    return *this;
  }

  ~BufferPoolReservation() {
    Resize(0_Size);
  };

  void Resize(Size newSize);
  void Merge(BufferPoolReservation&& other);
};

struct BufferEvictionNode {

};

class BufferPool {
public:
  explicit BufferPool(Size maxSize) :
      maxMemory_{maxSize}
      {}
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
  virtual auto EvictBlocks (Size extraMemory, Size memoryLimit) -> Eviction;

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
