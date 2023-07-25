//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\file_buffer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "port/file_handle.hh"
#include "storage/allocator.hh"

#include "common/macro.hh"
#include "common/type.hh"

namespace saturn {

enum class FileBufferType : u8 {
  BLOCK =
      1, // this kind of buffer is used to store PRESISTENT data like rows!

  MANAGED_BUFFER = 2, // a buffer managed by the buffer pool, it can be used to
                      // store TEMPORARY data! we may remember that the query
                      // execution in DBMS is potentially very space-consuming.
                      // We may need to spill sorting or aggregation results to
                      // disk when running, and load them back afterwards. This
                      // kind of buffer is used to store such TEMPORARY data.

  TINY_BUFFER = 3, // a tiny buffer
};

class FileBuffer {
  friend class BlockManager;
  friend class SingleFileBlockManager;
public:
  FileBuffer(Allocator &allocator, FileBufferType type, Size requestSize);
  FileBuffer(FileBuffer &source, FileBufferType type);
  virtual ~FileBuffer();

  Allocator &allocator;
  FileBufferType type;
  DatumPtr buffer = nullptr;
  Size limitSize = 0_Size; // max size user can write, may be slighly bigger than
                  // requested, because of alignment


  void Read(FileHandle &handle, Offset location) const;
  void Write(FileHandle &handle, Offset location) const;
  void Clear();
  void Resize(Size newSize);

  [[nodiscard]] auto GetAllocationSize() const -> Size { return internalSize; }

protected:
  DatumPtr internalBuffer = nullptr;
  Size internalSize = 0_Size;

private:
  void ReallocBuffer(Size newSize);
};

} // namespace saturn
