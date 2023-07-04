//===------------------------------------------===
// Saturn 2023
//
// Identification: include\port\unix_file_system.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// this file contains the unix file system.
//===----------------------------------------------------===
#pragma once

#include "port/file_system.hh"

namespace saturn {

class UnixFileSystem : public FileSystem {
  auto Open(string path, OpenFlags flags) -> unique_ptr<FileHandle> final;

  void
  Read(FileHandle &handle, void *buffer, Size nr_bytes, Offset location) final;

  void Write(FileHandle &handle,
             const void *buffer,
             Size nr_bytes,
             Offset location) final;

  auto Read(FileHandle &handle, void *buffer, Size nr_bytes)
      -> result<Size> final;

  auto Write(FileHandle &handle, const void *buffer, Size nr_bytes)
      -> result<Size> final;

  auto GetFileSize(FileHandle &handle) -> result<Size> final;
  auto GetLastModifiedTime(FileHandle &handle) -> time_t final;

  auto GetFileType(FileHandle &handle) -> FileType final;
  void Truncate(FileHandle &handle, Size new_size) final;
};

} // namespace saturn
