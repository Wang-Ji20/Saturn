//===------------------------------------------===
// Saturn 2023
//
// Identification: include\port\windows_file_system.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// this file contains the file system for windows.
//===----------------------------------------------------===

#pragma once

#include "port/file_system.hh"

namespace saturn {

class WindowsFileSystem final : public FileSystem {
public:
  auto Open(string path, OpenFlags flags) -> unique_ptr<FileHandle> final;
  void Remove(string path) final;

  void ReadAt(FileHandle &handle,
              void *buffer,
              Size nr_bytes,
              Offset location) final;

  void WriteAt(FileHandle &handle,
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

  void Seek(FileHandle &handle, Offset location) final;
  auto GetPosition(FileHandle &handle) -> Offset final;
};

} // namespace saturn
