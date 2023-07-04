//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/unix_file_system.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/unix_file_system.hh"

#include <unistd.h>

namespace saturn {

[[maybe_unused]] static void CheckValidOpenFlags(OpenFlags flags) {
  [[maybe_unused]] bool is_read = ContainFlags(flags, OpenFlags::READ);
}

auto UnixFileSystem::Open(string path, OpenFlags flags)
    -> unique_ptr<FileHandle> {

  return nullptr;
}

} // namespace saturn
