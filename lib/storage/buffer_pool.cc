//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/buffer_pool.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/buffer_pool.hh"
#include "common/logger.hh"

namespace saturn {

void BufferPoolReservation::Resize(Size newSize) {
  auto result = pool.IncreaseUsedMemory(newSize - size);
  CHECK(result.ok()) << result.message();
  size = newSize;
}

void BufferPoolReservation::Merge(BufferPoolReservation &&other) {
  size += other.size;
  other.size = 0_Size;
}

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
