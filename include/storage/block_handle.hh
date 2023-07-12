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
  Size size{0};
  BufferPool &pool;

  explicit BufferPoolReservation(BufferPool &bufferPool) : pool{bufferPool} {}
  BufferPoolReservation(BufferPool &bufferPool, Size size) : pool{bufferPool} {
    Resize(size);
  }
  DISALLOW_COPY(BufferPoolReservation);
  BufferPoolReservation(BufferPoolReservation &&other) noexcept
      : pool{other.pool} {
    size = other.size;
    other.size = 0_Size;
  }
  auto operator=(BufferPoolReservation &&other) noexcept
      -> BufferPoolReservation & {
    size = other.size;
    other.size = 0_Size;
    return *this;
  }

  ~BufferPoolReservation() { Resize(0_Size); };

  void Resize(Size newSize);
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

public:
  BlockHandle(BlockManager &manager, BlockId blockId);

  BlockManager &blockManager_;

private:
  static auto Load(shared_ptr<BlockHandle> &handle,
                   unique_ptr<FileBuffer> buffer = nullptr) -> BufferHandle;
  static auto CanUnload() -> bool;
  void Unload();

  mutable mutex lock_;
  atomic<Size> timestamp_;
  atomic<BlockStatus> status_;
  atomic<i32> readers_;
  const BlockId blockId_;
  Size memoryUsage_;
  BufferPoolReservation memoryCharge_;
  unique_ptr<FileBuffer> buffer_;
  bool canDestroy_;
};

} // namespace saturn
