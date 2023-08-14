//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\allocator.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------------===
// this file contains the allocator for memory.
// it serves as dependency injection for different malloc systems
// and provides a unified interface for memory allocation.
//
// https://www.huy.rocks/everyday/01-05-2022-zig-where-data-is-stored-and-how-to-choose-an-allocator
//
// NB. We require all heap memory allocation go through this.
// No Allocator, No dynamic memory allocation.
//
//===----------------------------------------------------------===

#pragma once

#include "common/logger.hh"
#include "common/macro.hh"
#include "common/optional.hh"
#include "common/shared_ptr.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

namespace saturn {

class Allocator;

class AllocatedData {
  friend class Allocator;

public:
  AllocatedData() = default;
  AllocatedData(Allocator &allocator, DatumPtr pointer, MemoryByte size);
  virtual ~AllocatedData() = default;

  // essential methods
  DISALLOW_COPY(AllocatedData);
  AllocatedData(AllocatedData &&) noexcept;
  auto operator=(AllocatedData &&) noexcept -> AllocatedData &;

private:
  optional<Allocator *> allocator_{};
  DatumPtr pointer_{};
  MemoryByte size_{};
};

using MallocFunction = std::function<DatumPtr(MemoryByte)>;
using FreeFunction = std::function<void(DatumPtr)>;
using ReallocFunction = std::function<DatumPtr(DatumPtr, MemoryByte)>;

class Allocator {
public:
  static constexpr const MemoryByte MAXIMUM_SIZE = 281474976710656ULL;
  static auto DefaultMalloc(MemoryByte size) -> DatumPtr {
    return DatumPtr(std::malloc(size));
  };

  static auto DefaultRealloc(DatumPtr pointer, MemoryByte size) -> DatumPtr {
    return DatumPtr(std::realloc(pointer, size));
  };

  static auto DefaultFree(DatumPtr pointer) -> void { std::free(pointer); };

  Allocator(MallocFunction mallocFunction = DefaultMalloc,
            FreeFunction freeFunction = DefaultFree,
            ReallocFunction reallocFunction = DefaultRealloc);

  DISALLOW_COPY_AND_MOVE(Allocator);

  // the template is a syntax sugar for casting
  // WARNING: template must not divide to declaration and definition
  template <typename T = Datum>
  auto AllocateData(MemoryByte size) -> T * {
    DCHECK(size > 0);
    DCHECK(size <= MAXIMUM_SIZE);
    auto *pointer = malloc_(size);
    DCHECK(pointer != nullptr);
    return reinterpret_cast<T*>(pointer);
  };

  template <typename T = Datum>
  auto FreeData(T *pointer, MemoryByte size) -> void {
    DCHECK(pointer != nullptr);
    free_(reinterpret_cast<Datum*>(pointer));
  }
  auto ReallocateData(DatumPtr pointer, MemoryByte size) -> DatumPtr;

  auto Allocate(MemoryByte size) -> AllocatedData;
  auto Free(AllocatedData &data) -> void;

  template <typename T, typename... Args>
  auto make_unique(Args &&...args) -> unique_ptr<T> {
    MemoryByte size = sizeof(T);
    T *ptr = AllocateData<T *>(size);
    return std::move(
        unique_ptr(new (ptr) T(std::forward<Args>(args)...), [&]() {
          ptr->~T();
          FreeData(ptr, size);
        }));
  }

  static auto GetDefaultAllocator() -> Allocator &;
  static auto GetDefaultAllocatorPointer() -> weak_ptr<Allocator>;

private:
  MallocFunction malloc_{};
  FreeFunction free_{};
  ReallocFunction realloc_{};
};

} // namespace saturn
