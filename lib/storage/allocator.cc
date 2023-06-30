//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/allocator.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/allocator.hh"
#include "common/logger.hh"

namespace saturn {

AllocatedData::AllocatedData(Allocator &allocator,
                             datum_ptr_t pointer,
                             idx_t size)
    : allocator_(&allocator), pointer_(pointer), size_(size) {
  DCHECK(pointer_ != nullptr);
};

AllocatedData::AllocatedData(AllocatedData &&other) noexcept
    : allocator_(std::move(other.allocator_)), pointer_(other.pointer_),
      size_(other.size_){};

} // namespace saturn