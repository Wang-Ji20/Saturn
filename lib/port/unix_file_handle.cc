//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/unix_file_handle.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#ifndef _WIN32

#include "port/unix_file_handle.hh"

namespace saturn {

void UnixFileHandle::Close() {
  if (fd == INVALID_FD) {
    return;
  }
  close(fd);
}

} // namespace saturn

#endif
