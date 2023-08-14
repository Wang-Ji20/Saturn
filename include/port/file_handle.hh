//===------------------------------------------===
// Saturn 2023
//
// Identification: include/port/file_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// unified interface for HANDLE, fd etc.
//===------------------------------------------------===

#pragma once

#include "common/macro.hh"
#include "common/result.hh"
#include "common/string.hh"
#include "common/type.hh"

namespace saturn {

class FileSystem;

enum class FileType {
  REGULAR,
  DIRECTORY,
  SOCKET,
  FIFO,
  SYMLINK,
  BLOCK,
  CHARACTER,
  UNKNOWN,
};

struct FileHandle {
public:
  FileHandle(FileSystem &fileSys, string path) : fs(fileSys), path(std::move(path)){};
  DISALLOW_COPY(FileHandle);
  virtual ~FileHandle() = default;

  CAST_TO_DERIVED(FileHandle);

  auto Read(void *buf, MemoryByte size) -> result<MemoryByte>;
  void ReadAt(void *buf, MemoryByte size, MemoryByte location);
  auto Write(const void *buf, MemoryByte size) -> result<MemoryByte>;
  void WriteAt(const void *buf, MemoryByte size, MemoryByte location);
  void Seek(MemoryByte location);
  void Reset();
  auto GetPosition() -> MemoryByte;
  void Sync();
  void Truncate(MemoryByte size);
  auto ReadLine() -> string;
  auto CanSeek() -> bool;
  auto OnDisk() -> bool;
  auto GetFileSize() -> result<MemoryByte>;
  auto GetType() -> FileType;

  virtual void Close() = 0;


  FileSystem &fs;
  string path;
};

} // namespace saturn
