//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/unix_file_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/unix_file_handle.hh"
#include <unistd.h>

namespace saturn {

void UnixFileHandle::Close() {
  if (fd == INVALID_FD) {
    return;
  }
  close(fd);
}

} // namespace saturn
