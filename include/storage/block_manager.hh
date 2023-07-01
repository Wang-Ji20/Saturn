//===------------------------------------------===
// Saturn 2023
//
// Identification: include\storage\block_manager.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===----------------------------------------------------===
// provides a unified interface for all block managers.
//===----------------------------------------------------===

#pragma once

#include "common/macro.hh"
#include "common/type.hh"
#include "storage/block.hh"

namespace saturn {

class BlockManager {
public:
  virtual ~BlockManager() = default;

  // essential methods
  DISALLOW_COPY(BlockManager);
  BlockManager(BlockManager &&) noexcept;
  auto operator=(BlockManager &&) noexcept -> BlockManager &;

  //===----------------------------------------------------===
  // allocate a block of memory with the given size.
  //===----------------------------------------------------===
  virtual auto allocate(Size size) -> Block = 0;

  //===----------------------------------------------------===
  // free the block of memory.
  //===----------------------------------------------------===
  virtual auto free(Block block) -> void = 0;

  //===----------------------------------------------------===
  // resize the block of memory.
  //===----------------------------------------------------===
  virtual auto resize(Block block, Size size) -> Block = 0;

  //===----------------------------------------------------===
  // get the size of the block.
  //===----------------------------------------------------===
  virtual auto size(Block block) -> Size = 0;

  //===----------------------------------------------------===
  // get the pointer of the block.
  //===----------------------------------------------------===
  virtual auto pointer(Block block) -> DatumPtr = 0;
};

} // namespace saturn
