//===------------------------------------------===
// Saturn 2023
//
// Identification: include/port/local_file_system.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "port/windows_file_system.hh"
#include "port/unix_file_system.hh"

namespace saturn {

using LocalFileSystem =
#ifdef _WIN32
    WindowsFileSystem;
#else
    UnixFileSystem;
#endif

} // namespace saturn
