//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\default_buffer_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// this file contains the default buffer manager in memory.
//===----------------------------------------------------===

#pragma once

#include "storage/buffer_manager.hh"

namespace saturn {

class Database;

class DefaultBufferManager : public BufferManager {
public:
  explicit DefaultBufferManager(Database &database);
  auto Pin(shared_ptr<BlockHandle> &handle) -> BufferHandle override;
  void Unpin(shared_ptr<BlockHandle> &handle) override;
  auto GetBufferPool() -> BufferPool & override;

private:
  Database &database_;
};

} // namespace saturn
