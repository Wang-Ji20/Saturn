//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/file_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/file_handle.hh"
#include "port/file_system.hh"

namespace saturn {

void FileHandle::ReadAt(void *buffer, MemoryByte size, MemoryByte location) {
  fs.ReadAt(*this, buffer, size, location);
}

void FileHandle::WriteAt(const void *buffer,
                         MemoryByte size,
                         MemoryByte location) {
  fs.WriteAt(*this, buffer, size, location);
}

auto FileHandle::Read(void *buffer, MemoryByte size) -> result<MemoryByte> {
  return fs.Read(*this, buffer, size);
}

auto FileHandle::Write(const void *buffer, MemoryByte size)
    -> result<MemoryByte> {
  return fs.Write(*this, buffer, size);
}

auto FileHandle::GetFileSize() -> result<MemoryByte> {
  return fs.GetFileSize(*this);
}

void FileHandle::Sync() { fs.FileSync(*this); }

} // namespace saturn
