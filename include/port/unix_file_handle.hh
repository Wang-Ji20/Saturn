//===------------------------------------------===
// Saturn 2023
//
// Identification: include/port/unix_file_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// UNIX syscall
//===------------------------------------------------===

#pragma once

#include "port/file_handle.hh"

namespace saturn {

struct UnixFileHandle : public FileHandle {
  static constexpr int INVALID_FD = -1;

public:
  UnixFileHandle(FileSystem &fs, string path, int fd)
      : FileHandle(fs, std::move(path)), fd(fd){};
  ~UnixFileHandle() final { UnixFileHandle::Close(); };

  int fd;
  void Close() final;
};

} // namespace saturn
