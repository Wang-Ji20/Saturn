//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/allocator/arena_allocator.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/allocator/arena_allocator.hh"

namespace saturn {

// use RAII to kill all Arenae after me
Arena::~Arena() {
  // delete previous arena, move new arena to delete handle
  for (; next != nullptr; next = std::move(next->next)) {
  }
}

} // namespace saturn
