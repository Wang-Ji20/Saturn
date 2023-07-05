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
//===----------------------------------------------------------===

#pragma once

#include "common/macro.hh"
#include "common/optional.hh"
#include "common/type.hh"
#include "common/unique_ptr.hh"

namespace saturn {

class Allocator;

class AllocatedData {
  friend class Allocator;

public:
  AllocatedData() = default;
  AllocatedData(Allocator &allocator, DatumPtr pointer, Size size);
  virtual ~AllocatedData() = default;

  // essential methods
  DISALLOW_COPY(AllocatedData);
  AllocatedData(AllocatedData &&) noexcept;
  auto operator=(AllocatedData &&) noexcept -> AllocatedData &;

private:
  optional<Allocator *> allocator_{};
  DatumPtr pointer_{};
  Size size_{};
};

using MallocFunction = std::function<DatumPtr(Size)>;
using FreeFunction = std::function<void(DatumPtr)>;
using ReallocFunction = std::function<DatumPtr(DatumPtr, Size)>;

class Allocator {
public:
  static constexpr const Size MAXIMUM_SIZE = 281474976710656_Size;
  static auto DefaultMalloc(Size size) -> DatumPtr {
    return DatumPtr(std::malloc(size));
  };

  static auto DefaultRealloc(DatumPtr pointer, Size size) -> DatumPtr {
    return DatumPtr(std::realloc(pointer, size));
  };

  static auto DefaultFree(DatumPtr pointer) -> void { std::free(pointer); };

  Allocator(MallocFunction mallocFunction = DefaultMalloc,
            FreeFunction freeFunction = DefaultFree,
            ReallocFunction reallocFunction = DefaultRealloc);

  DISALLOW_COPY_AND_MOVE(Allocator);

  auto AllocateData(Size size) -> DatumPtr;
  auto FreeData(DatumPtr pointer, Size size) -> void;
  auto Allocate(Size size) -> AllocatedData;
  auto Free(AllocatedData &data) -> void;

private:
  MallocFunction malloc_{};
  FreeFunction free_{};
  ReallocFunction realloc_{};
};

} // namespace saturn
