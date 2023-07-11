//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/buffer_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// buffer handle holds a space in memory.
// It is managed by buffer manager.
//
// It pin the data or unpin them automatically using RAII.
//
//===------------------------------------------------===

#pragma once

#include "storage/file_buffer.hh"

namespace saturn {

class BlockHandle;

class BufferHandle {
public:
  BufferHandle() = default;
  BufferHandle(shared_ptr<BlockHandle> handle, FileBuffer *node)
      : handle{std::move(handle)}, node{node} {};
  ~BufferHandle() { Destroy(); }
  DISALLOW_COPY(BufferHandle);
  BufferHandle(BufferHandle &&other) noexcept;
  auto operator=(BufferHandle &&other) noexcept -> BufferHandle &;

  [[nodiscard]] inline auto IsValid() const noexcept -> bool {
    return node != nullptr;
  }
  [[nodiscard]] inline auto Ptr() const noexcept -> DatumPtr {
    return node->buffer;
  }
  [[nodiscard]] inline auto Ptr() noexcept -> DatumPtr { return node->buffer; }
  [[nodiscard]] inline auto GetFileBuffer() noexcept -> FileBuffer & {
    return *node;
  }
  auto Destroy() -> void;

  [[nodiscard]] inline auto GetBlockHandle() const noexcept
      -> const shared_ptr<BlockHandle> & {
    return handle;
  }

private:
  //! The block handle
  shared_ptr<BlockHandle> handle{nullptr};
  //! The managed buffer node
  FileBuffer *node{nullptr};
};

} // namespace saturn
