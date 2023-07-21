//===------------------------------------------===
// Saturn 2023
//
// Identification: include/main/saturn.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "catalog/database.hh"
#include "common/shared_ptr.hh"

#include "main/config.hh"

namespace saturn {

class Saturn {
public:
  // empty path means in-memory database
  // config is optional, if not provided, we will use default config
  explicit Saturn(string path = "", DBConfig *config = nullptr);

private:
  shared_ptr<Database> database_;
};

} // namespace saturn
