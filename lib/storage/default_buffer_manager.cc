//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/default_buffer_manager.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/default_buffer_manager.hh"
#include "catalog/database.hh"
#include "common/exception.hh"

namespace saturn {

DefaultBufferManager::DefaultBufferManager(Database &database)
    : database_(database), bufferpool_(database.GetBufferPool()) {}

auto DefaultBufferManager::GetBufferPool() -> BufferPool & {
  return bufferpool_;
}

auto DefaultBufferManager::EvictBlocksOrThrow(MemoryByte evictMemory,
                                              unique_ptr<FileBuffer> *buffer)
    -> BufferPoolReservation {
  auto [ok, reservation, reuseBuffer] =
      bufferpool_.EvictBlocks(evictMemory, bufferpool_.GetMaxMemory());
  if (!ok) {
    throw InternalException("out of memory!");
  }
  if (reuseBuffer.ok()) {
    *buffer = std::move(reuseBuffer.value());
  }
  return std::move(reservation);
}

auto DefaultBufferManager::Pin(shared_ptr<BlockHandle> &handle)
    -> BufferHandle {
  MemoryByte reqMem;
  {
    scoped_lock<mutex> latch(handle->lock_);
    if (handle->status_ == BlockStatus::LOADED) {
      handle->readers_++;
      return handle->Load(handle);
    }
    reqMem = handle->memoryUsage_;
  }
  // find a little place for our block.
  unique_ptr<FileBuffer> buffer;
  auto reservation = EvictBlocksOrThrow(reqMem, &buffer);

  scoped_lock<mutex> latch(handle->lock_);
  // maybe someone else has loaded the block when we are evicting.
  if (handle->status_ == BlockStatus::LOADED) {
    handle->readers_++;
    return handle->Load(handle);
  }

  DCHECK(handle->readers_ == 0);
  handle->readers_++;
  auto buf = handle->Load(handle, std::move(buffer));
  handle->memoryCharge_ = std::move(reservation);
  return buf;
}

void DefaultBufferManager::Unpin(shared_ptr<BlockHandle> &handle) {
  scoped_lock<mutex> latch(handle->lock_);
  DCHECK(handle->readers_ > 0);
  handle->readers_--;
  if (handle->readers_ == 0) {
    bufferpool_.AddToEvictionQueue(handle);
  }
}

} // namespace saturn
