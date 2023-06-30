//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\allocator.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// this file contains the allocator for memory.
// it serves as dependency injection for different malloc systems
// and provides a unified interface for memory allocation.
//===------------------------------------------------===

#pragma once

#include "common/macro.hh"
#include "common/optional.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

namespace saturn {

class Allocator;

class AllocatedData {
public:
  AllocatedData() = default;
  AllocatedData(Allocator &allocator, datum_ptr_t pointer, idx_t size);
  virtual ~AllocatedData() = default;

  // essential methods
  DISALLOW_COPY(AllocatedData);
  AllocatedData(AllocatedData &&) noexcept;
  auto operator=(AllocatedData &&) noexcept -> AllocatedData &;

private:
  optional<Allocator *> allocator_{};
  datum_ptr_t pointer_{};
  idx_t size_{};
};

class Allocator {
    static constexpr const idx_t MAXIMUM_SIZE = 281474976710656_idx_t;

};

} // namespace saturn