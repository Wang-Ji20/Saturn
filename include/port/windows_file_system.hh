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
              MemoryByte nr_bytes,
              MemoryByte location) final;

  void WriteAt(FileHandle &handle,
               const void *buffer,
               MemoryByte nr_bytes,
               MemoryByte location) final;

  auto Read(FileHandle &handle, void *buffer, MemoryByte nr_bytes)
      -> result<MemoryByte> final;

  auto Write(FileHandle &handle, const void *buffer, MemoryByte nr_bytes)
      -> result<MemoryByte> final;

  auto GetFileSize(FileHandle &handle) -> result<MemoryByte> final;
  auto GetLastModifiedTime(FileHandle &handle) -> time_t final;

  auto GetFileType(FileHandle &handle) -> FileType final;
  void Truncate(FileHandle &handle, MemoryByte new_size) final;

  void Seek(FileHandle &handle, MemoryByte location) final;
  auto GetPosition(FileHandle &handle) -> MemoryByte final;

  void FileSync(FileHandle &handle) final;

  auto ExistFile(const string &path) -> bool final;
};

} // namespace saturn
