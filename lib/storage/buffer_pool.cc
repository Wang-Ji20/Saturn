//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/buffer_pool.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/buffer_pool.hh"

namespace saturn {

auto BufferPool::IncreaseUsedMemory(Size size) -> Status {
  // https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
  auto usedMemory = usedMemory_.load();
  while (!usedMemory_.compare_exchange_weak(usedMemory, usedMemory + size)) {
    if (usedMemory + size > maxMemory_.load()) {
      return absl::ResourceExhaustedError("Buffer pool is full.");
    }
  }
  return absl::OkStatus();
}

auto BufferPool::SetMaxMemory(Size size) -> Status {
  auto maxMemory = maxMemory_.load();
  while (!maxMemory_.compare_exchange_weak(maxMemory, size)) {
    if (maxMemory < usedMemory_.load()) {
      return absl::ResourceExhaustedError("Buffer pool is full.");
    }
  }
  return absl::OkStatus();
}

} // namespace saturn
