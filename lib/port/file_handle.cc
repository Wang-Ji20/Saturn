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

void FileHandle::ReadAt(void *buffer, Size nr_bytes, Offset location) {
  fs.ReadAt(*this, buffer, nr_bytes, location);
}

void FileHandle::WriteAt(const void *buffer, Size nr_bytes, Offset location) {
  fs.WriteAt(*this, buffer, nr_bytes, location);
}

auto FileHandle::Read(void *buffer, Size nr_bytes) -> result<Size> {
  return fs.Read(*this, buffer, nr_bytes);
}

auto FileHandle::Write(const void *buffer, Size nr_bytes) -> result<Size> {
  return fs.Write(*this, buffer, nr_bytes);
}

auto FileHandle::GetFileSize() -> result<Size> { return fs.GetFileSize(*this); }

} // namespace saturn
