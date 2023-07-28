//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/port/file_system.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/file_system.hh"

namespace saturn {

void FileSystem::Reset(FileHandle &handle) { Seek(handle, 0); }

} // namespace saturn
