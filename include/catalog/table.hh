//===------------------------------------------===
// Saturn 2023
//
// Identification: include/catalog/table.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "catalog/catalog_entry.hh"
#include "common/shared_ptr.hh"

namespace saturn {

class Table;

class TableEntry : public CatalogEntry {
private:
  shared_ptr<Table> physicalTable_;
};

} // namespace saturn
