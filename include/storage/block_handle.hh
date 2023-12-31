//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/block_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/atomic.hh"
#include "common/macro.hh"
#include "common/mutex.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

#include "storage/file_buffer.hh"

namespace saturn {

class BufferPool;
class BufferHandle;
class BlockManager;

struct BufferPoolReservation {
  MemoryByte size{0};
  BufferPool &pool;

  explicit BufferPoolReservation(BufferPool &bufferPool) : pool{bufferPool} {}
  BufferPoolReservation(BufferPool &bufferPool, MemoryByte size) : pool{bufferPool} {
    Resize(size);
  }
  DISALLOW_COPY(BufferPoolReservation);
  BufferPoolReservation(BufferPoolReservation &&other) noexcept
      : pool{other.pool} {
    size = other.size;
    other.size = 0ULL;
  }
  auto operator=(BufferPoolReservation &&other) noexcept
      -> BufferPoolReservation & {
    size = other.size;
    other.size = 0ULL;
    return *this;
  }

  ~BufferPoolReservation() { Resize(0ULL); };

  void Resize(MemoryByte newSize);
  void Merge(BufferPoolReservation &&other);
};

enum class BlockStatus : u8 {
  LOADED,
  UNLOADED,
};

class BlockHandle {
  friend struct BufferEvictionNode;
  friend struct BufferPoolReservation;
  friend class BufferPool;
  friend class DefaultBufferManager;

public:
  BlockHandle(BlockManager &manager, BlockId blockId);
  ~BlockHandle();

  BlockManager &blockManager_;

private:
  static auto Load(shared_ptr<BlockHandle> &handle,
                   unique_ptr<FileBuffer> buffer = nullptr) -> BufferHandle;
  auto CanUnload() -> bool;
  auto Unload() -> unique_ptr<FileBuffer>;

  mutable mutex lock_;
  atomic<MemoryByte> timestamp_;
  atomic<BlockStatus> status_;
  atomic<i32> readers_;
  const BlockId blockId_;
  MemoryByte memoryUsage_;
  BufferPoolReservation memoryCharge_;
  unique_ptr<FileBuffer> buffer_;
};

} // namespace saturn
