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

class FileSystem {
public:
  virtual ~FileSystem();

  virtual auto Open(string path, OpenFlags flags) -> unique_ptr<FileHandle>;

  virtual void
  Read(FileHandle &handle, void *buffer, Size nr_bytes, Offset location);

  virtual void
  Write(FileHandle &handle, const void *buffer, Size nr_bytes, Offset location);

  virtual auto Read(FileHandle &handle, void *buffer, Size nr_bytes)
      -> result<Size>;

  virtual auto Write(FileHandle &handle, const void *buffer, Size nr_bytes)
      -> result<Size>;

  virtual auto GetFileSize(FileHandle &handle) -> result<Size>;
  virtual auto GetLastModifiedTime(FileHandle &handle) -> time_t;

  virtual auto GetFileType(FileHandle &handle) -> FileType;
  virtual void Truncate(FileHandle &handle, Size new_size);
};

} // namespace saturn
