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
      1, // this kind of buffer is used to store PRESISTENT data like tables!

  MANAGED_BUFFER = 2, // a buffer managed by the buffer pool, it can be used to
                      // store TEMPORARY data! we may remember that the query
                      // execution in DBMS is potentially very space-consuming.
                      // We may need to spill sorting or aggregation results to
                      // disk when running, and load them back afterwards. This
                      // kind of buffer is used to store such TEMPORARY data.

  TINY_BUFFER = 3, // a tiny buffer
};

class FileBuffer {
public:
  FileBuffer(Allocator &allocator, FileBufferType type, Size requestSize);
  FileBuffer(FileBuffer &source, FileBufferType type);
  virtual ~FileBuffer();

  Allocator &allocator;
  FileBufferType type;
  DatumPtr buffer;
  Size limitSize; // max size user can write, may be slighly bigger than
                  // requested, because of alignment

public:
  void Read(FileHandle &handle, Offset location);
  void Write(FileHandle &handle, Offset location);
  void Clear();
  void Resize(Size newSize);

protected:
  DatumPtr internalBuffer;
  Size internalSize;

private:
  void ReallocBuffer(Size newSize);
  void Init();
};

} // namespace saturn
