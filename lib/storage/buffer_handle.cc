//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/buffer_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/buffer_handle.hh"

namespace saturn {

BufferHandle::BufferHandle(BufferHandle &&other) noexcept {
  std::swap(node, other.node);
  std::swap(handle, other.handle);
}

auto BufferHandle::operator=(BufferHandle &&other) noexcept -> BufferHandle & {
  std::swap(node, other.node);
  std::swap(handle, other.handle);
  return *this;
}

// TODO
auto BufferHandle::Destroy() -> void {}

} // namespace saturn
