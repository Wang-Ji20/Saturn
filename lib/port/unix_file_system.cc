//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/unix_file_system.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef _WIN32

#include "port/unix_file_system.hh"
#include "common/logger.hh"
#include "port/unix_file_handle.hh"

extern "C" {
#include <sys/stat.h>
#include <sys/types.h>
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

auto UnixFileSystem::Open(string path, OpenFlags flags)
    -> unique_ptr<FileHandle> {
  DCHECK(CheckValidOpenFlags(
      flags)); // because open is internal, no need to check in release mode
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
    // OSX does not have O_DIRECT, instead we need to use fcntl afterwards to
    // support direct IO
    openflags |= O_SYNC;
#else
    openflags |= O_DIRECT | O_SYNC;
#endif
  }

  int fd = open(path.c_str(), openflags, 0644);
  PCHECK(fd > -1); // considered a severe error
  struct stat fileStat;
  PCHECK(fstat(fd, &fileStat) == 0); // considered a severe error
  return make_unique<UnixFileHandle>(*this, path, fd, fileStat);
}

void UnixFileSystem::Remove(string path) { PCHECK(unlink(path.c_str()) == 0); }

void UnixFileSystem::ReadAt(FileHandle &handle,
                            void *buffer,
                            Size nr_bytes,
                            Offset offset) {
  DCHECK(buffer);
  DCHECK(nr_bytes > 0);
  int fd = handle.Cast<UnixFileHandle>().fd;
  int nr_read = pread(fd, buffer, nr_bytes, offset);
  PCHECK(nr_read > -1);
  PCHECK(Size(nr_read) == nr_bytes); // considered a severe error
}

void UnixFileSystem::WriteAt(FileHandle &handle,
                             const void *buffer,
                             Size nr_bytes,
                             Offset offset) {
  DCHECK(buffer);
  DCHECK(nr_bytes > 0);
  int fd = handle.Cast<UnixFileHandle>().fd;
  int nr_written = pwrite(fd, buffer, nr_bytes, offset);
  PCHECK(nr_written > -1);
  PCHECK(Size(nr_written) == nr_bytes); // considered a severe error
}

auto UnixFileSystem::Read(FileHandle &handle, void *buffer, Size nr_bytes)
    -> result<Size> {
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

auto UnixFileSystem::Write(FileHandle &handle,
                           const void *buffer,
                           Size nr_bytes) -> result<Size> {
  DCHECK(buffer);
  DCHECK(nr_bytes > 0);
  int fd = handle.Cast<UnixFileHandle>().fd;
  int nr_written = write(fd, buffer, nr_bytes);
  if (nr_written < 0) {
    PLOG(WARNING) << "write failed";
    return absl::ResourceExhaustedError("write failed");
  }
  return Size(nr_written);
}

auto UnixFileSystem::GetFileSize(FileHandle &handle) -> result<Size> {
  struct stat fileStat = handle.Cast<UnixFileHandle>().fileStat;
  return Size(fileStat.st_size);
}

auto UnixFileSystem::GetLastModifiedTime(FileHandle &handle) -> time_t {
  struct stat fileStat = handle.Cast<UnixFileHandle>().fileStat;
  return fileStat.st_mtime;
}

auto UnixFileSystem::GetFileType(FileHandle &handle) -> FileType {
  struct stat fileStat = handle.Cast<UnixFileHandle>().fileStat;

  if (S_ISREG(fileStat.st_mode)) {
    return FileType::REGULAR;
  }
  if (S_ISDIR(fileStat.st_mode)) {
    return FileType::DIRECTORY;
  }
  if (S_ISLNK(fileStat.st_mode)) {
    return FileType::SYMLINK;
  }
  if (S_ISCHR(fileStat.st_mode)) {
    return FileType::CHARACTER;
  }
  if (S_ISBLK(fileStat.st_mode)) {
    return FileType::BLOCK;
  }
  if (S_ISFIFO(fileStat.st_mode)) {
    return FileType::FIFO;
  }
  if (S_ISSOCK(fileStat.st_mode)) {
    return FileType::SOCKET;
  }

  return FileType::UNKNOWN;
}

void UnixFileSystem::Truncate(FileHandle &handle, Size size) {
  int fd = handle.Cast<UnixFileHandle>().fd;
  PCHECK(ftruncate(fd, size) == 0);
}

static void setFilePointer(int fd, Offset offset) {
  PCHECK(lseek(fd, offset, SEEK_SET) == offset);
}

static auto getFilePointer(int fd) -> Offset {
  auto offset = Offset(lseek(fd, 0, SEEK_CUR));
  PCHECK(offset >= 0);
  return offset;
}

void UnixFileSystem::Seek(FileHandle &handle, Offset offset) {
  int fd = handle.Cast<UnixFileHandle>().fd;
  setFilePointer(fd, offset);
}

auto UnixFileSystem::GetPosition(FileHandle &handle) -> Offset {
  int fd = handle.Cast<UnixFileHandle>().fd;
  return getFilePointer(fd);
}

} // namespace saturn

#endif // _WIN32
