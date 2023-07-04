//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/unix_file_system.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/unix_file_system.hh"
#include "port/unix_file_handle.hh"
#include "common/logger.hh"

extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

namespace saturn {

static auto CheckValidOpenFlags(OpenFlags flags) -> bool {
  bool is_read = ContainFlags(flags, OpenFlags::READ);
  bool is_write = ContainFlags(flags, OpenFlags::WRITE);
  bool is_create = ContainFlags(flags, OpenFlags::CREATE);
  bool is_truncate = ContainFlags(flags, OpenFlags::CREATE_TRUNCATE);
  bool is_append = ContainFlags(flags, OpenFlags::APPEND);
  DCHECK(is_read || is_write);
  if (is_create || is_truncate || is_append) {
    DCHECK(is_write);
  }
  if (is_truncate) {
    DCHECK(!is_create);
  }
  if (is_create) {
    DCHECK(!is_truncate);
  }
  return true;
}

auto UnixFileSystem::Open(string path, OpenFlags flags) -> unique_ptr<FileHandle> {
  DCHECK(CheckValidOpenFlags(flags)); // because open is internal, no need to check in release mode
  int openflags = 0;
  if (ContainFlagsAnd(flags, OpenFlags::READ, OpenFlags::WRITE)) {
    openflags |= O_RDWR;
  } else if (ContainFlags(flags, OpenFlags::READ)) {
    openflags |= O_RDONLY;
  } else if (ContainFlags(flags, OpenFlags::WRITE)) {
    openflags |= O_WRONLY;
  } else {
    CHECK(false) << "invalid open flags. must contain read or write";
  }

  if (ContainFlags(flags, OpenFlags::WRITE)) {
    openflags |= O_CLOEXEC;
    if (ContainFlags(flags, OpenFlags::CREATE)) {
      openflags |= O_CREAT;
    } else if (ContainFlags(flags, OpenFlags::CREATE_TRUNCATE)) {
      openflags |= O_TRUNC | O_CREAT;
    }
    if (ContainFlags(flags, OpenFlags::APPEND)) {
      openflags |= O_APPEND;
    }
  }

  if (ContainFlags(flags, OpenFlags::DIRECT_IO)) {
#if defined(__DARWIN__) || defined(__APPLE__) || defined(__OpenBSD__)
		// OSX does not have O_DIRECT, instead we need to use fcntl afterwards to support direct IO
		openflags |= O_SYNC;
#else
		openflags |= O_DIRECT | O_SYNC;
#endif
  }

  int fd = open(path.c_str(), openflags, 0644);
  PCHECK(fd); // considered a severe error
  return make_unique<UnixFileHandle>(*this, path, fd);
}

auto UnixFileSystem::Read(FileHandle &handle, void *buffer, Size nr_bytes) -> result<Size> {
  DCHECK(buffer);
  DCHECK(nr_bytes > 0);
  int fd = handle.Cast<UnixFileHandle>().fd;
  int nr_read = read(fd, buffer, nr_bytes);
  if (nr_read < 0) {
    PLOG(WARNING) << "read failed";
    return absl::ResourceExhaustedError("read failed");
  }
  return Size(nr_read);
}

} // namespace saturn
