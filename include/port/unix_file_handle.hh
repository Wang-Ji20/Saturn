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

#ifndef _WIN32

#include "port/file_handle.hh"

extern "C" {
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
}

namespace saturn {

struct UnixFileHandle final : public FileHandle {
  static constexpr int INVALID_FD = -1;

public:
  UnixFileHandle(FileSystem &fileSys, string path, int UNIXfd, struct stat fileStat)
      : FileHandle{fileSys, std::move(path)}, fd{UNIXfd}, fileStat{fileStat} {};
  ~UnixFileHandle() final { UnixFileHandle::Close(); };

  int fd;
  struct stat fileStat;
  void Close() final;
};

} // namespace saturn

#endif // _WIN32
