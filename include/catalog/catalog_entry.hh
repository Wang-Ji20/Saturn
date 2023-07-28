//===------------------------------------------===
// Saturn 2023
//
// Identification: include/catalog/catalog_entry.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "catalog/catalog_defs.hh"

namespace saturn {

class CatalogEntry {
public:
  CatalogEntry() = default;
  virtual ~CatalogEntry() = default;

private:
  [[maybe_unused]] CatalogEntryType type_ = CatalogEntryType::INVALID;
};

} // namespace saturn
