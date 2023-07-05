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
  FileHandle(FileSystem &fs, string path) : fs(fs), path(std::move(path)){};
  DISALLOW_COPY(FileHandle);
  virtual ~FileHandle() = default;

  CAST_TO_DERIVED(FileHandle);

  auto Read(void *buf, Size size) -> result<Size>;
  void ReadAt(void *buf, Size size, Offset location);
  auto Write(const void *buf, Size size) -> result<Size>;
  void WriteAt(const void *buf, Size size, Offset location);
  void Seek(Offset location);
  void Reset();
  auto GetPosition() -> Offset;
  void Sync();
  void Truncate(Size size);
  auto ReadLine() -> string;
  auto CanSeek() -> bool;
  auto OnDisk() -> bool;
  auto GetFileSize() -> result<Size>;
  auto GetType() -> FileType;

  virtual void Close() = 0;

public:
  FileSystem &fs;
  string path;
};

struct WindowsFileHandle;
struct UnixFileHandle;

using LocalFileHandle =
#ifdef _WIN32
    WindowsFileHandle;
#else
    UnixFileHandle;
#endif

} // namespace saturn
