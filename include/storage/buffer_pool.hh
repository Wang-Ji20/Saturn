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

namespace saturn {

class BufferPool {
public:
  explicit BufferPool(Size maxSize) :
      maxMemory_{maxSize}
      {}
  inline auto GetMaxMemory() -> Size { return Size(maxMemory_); }
  inline auto GetUsedMemory() -> Size { return Size(usedMemory_); }
  auto IncreaseUsedMemory(Size size) -> Status;
  auto SetMaxMemory(Size size) -> Status;

private:
  atomic<Size> maxMemory_;
  atomic<Size> usedMemory_ {0_Size};
};

} // namespace saturn
