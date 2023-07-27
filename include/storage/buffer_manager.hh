//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\buffer_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// this file contains the buffer manager in memory.
//===----------------------------------------------------===

#pragma once

#include "storage/buffer_handle.hh"

namespace saturn {

class BufferPool;

class BufferManager {
public:
  virtual auto GetBufferPool() -> BufferPool & = 0;
  virtual auto Pin(shared_ptr<BlockHandle> &handle) -> BufferHandle = 0;
  virtual void Unpin(shared_ptr<BlockHandle> &handle) = 0;
};

} // namespace saturn
