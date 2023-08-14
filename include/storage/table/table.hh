//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/table/table.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/mutex.hh"

namespace saturn {

/// class Table - physical table. stored in disk.
class Table {
private:
  mutable mutex latch_;
};

} // namespace saturn
