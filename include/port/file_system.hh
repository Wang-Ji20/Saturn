//===------------------------------------------===
// Saturn 2023
//
// Identification: include\port\file_system.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// provides a unified interface for all file systems.
//===----------------------------------------------------===

#pragma once

#include "common/result.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"
#include "port/file_handle.hh"

namespace saturn {

SATURN_ENUM_CLASS(OpenFlags, u8){
    READ = 1 << 0,
    WRITE = 1 << 1,
    CREATE = 1 << 2,
    CREATE_TRUNCATE = 1 << 3,
    APPEND = 1 << 4,
    DIRECT_IO = 1 << 5,
};

// NOTE: virtual function is needed to define. only pure virtual function is
// allowed to contain only declarations.
class FileSystem {
public:
  virtual ~FileSystem() = default;
  virtual auto Open(string path, OpenFlags flags) -> unique_ptr<FileHandle> = 0;
  virtual void Remove(string path) = 0;

  // Read exactly nr_bytes from the file at location, terminate when cannot read
  // that much.
  virtual void
  ReadAt(FileHandle &handle, void *buffer, Size nr_bytes, Offset location) = 0;

  // Write exactly nr_bytes to the file at location, terminate when cannot write
  // that much.
  virtual void WriteAt(FileHandle &handle,
                       const void *buffer,
                       Size nr_bytes,
                       Offset location) = 0;

  virtual auto Read(FileHandle &handle, void *buffer, Size nr_bytes)
      -> result<Size> = 0;

  virtual auto Write(FileHandle &handle, const void *buffer, Size nr_bytes)
      -> result<Size> = 0;

  virtual auto GetFileSize(FileHandle &handle) -> result<Size> = 0;
  virtual auto GetLastModifiedTime(FileHandle &handle) -> time_t = 0;

  virtual auto GetFileType(FileHandle &handle) -> FileType = 0;
  virtual void Truncate(FileHandle &handle, Size new_size) = 0;

  virtual void Seek(FileHandle &handle, Offset location) = 0;
  void Reset(FileHandle &handle);
  virtual auto GetPosition(FileHandle &handle) -> Offset = 0;
};

} // namespace saturn
