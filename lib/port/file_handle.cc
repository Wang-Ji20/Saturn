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

void FileHandle::ReadAt(void *buffer, Size size, Offset location) {
  fs.ReadAt(*this, buffer, size, location);
}

void FileHandle::WriteAt(const void *buffer, Size size, Offset location) {
  fs.WriteAt(*this, buffer, size, location);
}

auto FileHandle::Read(void *buffer, Size size) -> result<Size> {
  return fs.Read(*this, buffer, size);
}

auto FileHandle::Write(const void *buffer, Size size) -> result<Size> {
  return fs.Write(*this, buffer, size);
}

auto FileHandle::GetFileSize() -> result<Size> { return fs.GetFileSize(*this); }

} // namespace saturn
