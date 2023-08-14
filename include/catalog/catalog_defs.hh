//===------------------------------------------===
// Saturn 2023
//
// Identification: include/catalog/catalog_defs.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/type.hh"

namespace saturn {

enum class CatalogEntryType : u8 {
  INVALID = 0,
  TABLE = 1,
  INDEX = 2,
  SCHEMA = 3,
  VIEW = 4,
  TYPE = 5,
  DATABASE = 6,
  FUNCTION = 100,
};

} // namespace saturn
