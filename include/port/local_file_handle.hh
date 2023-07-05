//===------------------------------------------===
// Saturn 2023
//
// Identification: include/port/local_file_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "port/windows_file_handle.hh"
#include "port/unix_file_handle.hh"

namespace saturn {

using LocalFileHandle =
#ifdef _WIN32
    WindowsFileHandle;
#else
    UnixFileHandle;
#endif

} // namespace saturn
