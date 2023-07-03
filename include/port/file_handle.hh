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
#include "common/string.hh"
#include "common/type.hh"
#include <type_traits>

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
  FileHandle(FileSystem &fs, string path) : fs(fs), path(std::move(path)){};
  DISALLOW_COPY(FileHandle);
  virtual ~FileHandle();

  CAST_TO_DERIVED(FileHandle);

  auto Read(void *buf, Size size) -> i64;
  void Read(void *buf, Size size, Size location);
  auto Write(const void *buf, Size size) -> i64;
  void Write(const void *buf, Size size, Size location);
  void Seek(Size location);
  void Reset();
  auto Position() -> Size;
  void Sync();
  void Truncate(i64 size);
  auto ReadLine() -> string;
  auto CanSeek() -> bool;
  auto OnDisk() -> bool;
  auto Size() -> i64;
  auto Type() -> FileType;

  virtual void Close() = 0;

public:
  FileSystem &fs;
  string path;
};

} // namespace saturn
