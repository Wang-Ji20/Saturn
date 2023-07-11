//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/block_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/block_handle.hh"
#include "storage/buffer_handle.hh"

namespace saturn {

// TODO
auto BlockHandle::Load(shared_ptr<BlockHandle> &handle,
                       unique_ptr<FileBuffer> buffer) -> BufferHandle {
  return BufferHandle{};
}

// TODO
auto BlockHandle::Unload() -> void {}

// TODO
auto BlockHandle::CanUnload() -> bool { return false; }

} // namespace saturn
