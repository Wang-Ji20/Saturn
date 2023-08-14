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
#include "common/type.hh"
#include <cstdlib>
#include <utility>

namespace saturn {

AllocatedData::AllocatedData(Allocator &allocator,
                             DatumPtr pointer,
                             MemoryByte size)
    : allocator_(&allocator), pointer_(pointer), size_(size) {
  DCHECK(pointer_ != nullptr);
};

AllocatedData::AllocatedData(AllocatedData &&other) noexcept
    : allocator_(other.allocator_),
      pointer_(other.pointer_),
      size_(other.size_){};

//===----------------------------------------------------===
// Allocator
//===----------------------------------------------------===

Allocator::Allocator(MallocFunction mallocFunction,
                     FreeFunction freeFunction,
                     ReallocFunction reallocFunction)
    : malloc_(std::move(mallocFunction)),
      free_(std::move(freeFunction)),
      realloc_(std::move(reallocFunction)) {
  DCHECK(malloc_ != nullptr);
  DCHECK(free_ != nullptr);
  DCHECK(realloc_ != nullptr);
};

auto Allocator::Allocate(MemoryByte size) -> AllocatedData {
  DCHECK(size > 0);
  DCHECK(size <= MAXIMUM_SIZE);
  auto *pointer = malloc_(size);
  DCHECK(pointer != nullptr);
  return AllocatedData{*this, pointer, size};
};

auto Allocator::Free(AllocatedData &data) -> void {
  DCHECK(data.pointer_ != nullptr);
  free_(data.pointer_);
};

auto Allocator::ReallocateData(DatumPtr pointer, MemoryByte size) -> DatumPtr {
  DCHECK(pointer != nullptr);
  DCHECK(size > 0);
  DCHECK(size <= MAXIMUM_SIZE);
  auto *newPointer = realloc_(pointer, size);
  DCHECK(newPointer != nullptr);
  return newPointer;
}

auto Allocator::GetDefaultAllocator() -> Allocator & {
  auto defaultAllocator = GetDefaultAllocatorPointer().lock();
  return *defaultAllocator;
};

auto Allocator::GetDefaultAllocatorPointer() -> weak_ptr<Allocator> {
  static auto defaultAllocatorPointer = make_shared<Allocator>();
  return defaultAllocatorPointer;
};

} // namespace saturn
