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
#include "storage/allocator.hh"
#include "storage/buffer_pool.hh"

namespace saturn {

struct DBConfig {
  // do not use system cache provided by OS
  bool useDirectIO = false;

  // what file system we are using. local/network/...
  unique_ptr<FileSystem> fileSystem;

  // what memory allocator we are using..
  unique_ptr<Allocator> allocator;

  // buffer pool
  unique_ptr<BufferPool> bufferPool;
};

};
