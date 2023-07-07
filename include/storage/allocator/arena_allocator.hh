//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/allocator/arena_allocator.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/unique_ptr.hh"
#include "storage/allocator.hh"

namespace saturn {

struct Arena {
  Arena(Allocator &allocator, Size maxSize)
      : data{allocator.Allocate(maxSize)}, maxSize{maxSize},
        curPosition{0}, next{nullptr} {}

  ~Arena();

  AllocatedData data;
  Size maxSize;
  Offset curPosition;

  unique_ptr<Arena> next;
  Arena *prev{};
};

class ArenaAllocator : public Allocator {
public:
  ArenaAllocator(Allocator &base, Size capacity)
      : base_{base}, capacity_{capacity} {}

private:
  [[maybe_unused]] Allocator& base_;
  [[maybe_unused]] Size capacity_;
  [[maybe_unused]] unique_ptr<Arena> head_;
  [[maybe_unused]] Arena *tail_{};
};

} // namespace saturn
