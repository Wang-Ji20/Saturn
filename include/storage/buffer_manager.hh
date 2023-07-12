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

namespace saturn {

class BufferPool;

class BufferManager {
public:
  virtual auto GetBufferPool() -> BufferPool & = 0;
};

} // namespace saturn
