//===------------------------------------------===
// Saturn 2023
//
// Identification: include/main/config.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "port/file_system.hh"
#include "common/unique_ptr.hh"

namespace saturn {

struct DBConfig {
  // do not use system cache provided by OS
  bool useDirectIO = false;

  // what file system we are using. local/network/...
  unique_ptr<FileSystem> fileSystem;
};

};
