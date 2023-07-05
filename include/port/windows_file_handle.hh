//===------------------------------------------===
// Saturn 2023
//
// Identification: include/port/windows_file_handle.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// Windows syscall
//===------------------------------------------------===

#pragma once

#ifdef _WIN32

#include "port/file_handle.hh"

extern "C" {
#include <windows.h>
}

namespace saturn {

struct WindowsFileHandle final : public FileHandle {
  static constexpr HANDLE INVALID_HANDLE = INVALID_HANDLE_VALUE;
};

} // namespace saturn

#endif // _WIN32
