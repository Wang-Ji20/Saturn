//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\file_buffer.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "storage/allocator.hh"
#include "port/file_handle.hh"

#include "common/type.hh"
#include "common/macro.hh"

namespace saturn {

SATURN_ENUM_CLASS(FileBufferType, u8) {
    BLOCK = 1, // a small piece of free memory
    MANAGED_BUFFER = 2, // a buffer managed by the buffer pool
    TINY_BUFFER = 3, // a tiny buffer
};

class FileBuffer {
public:
    FileBuffer(Allocator& allocator, FileBufferType type, Size requestSize);
    FileBuffer(FileBuffer& source, FileBufferType type);
    virtual ~FileBuffer();

    Allocator& allocator;
    FileBufferType type;
    DatumPtr buffer;
    Size limitSize; // max size user can write, may be slighly bigger than requested, because of alignment

public:
  void Read(FileHandle& handle, Offset location);
  void Write(FileHandle& handle, Offset location);
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
