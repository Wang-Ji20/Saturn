//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\macro.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===--------------------------------------===
// this file contains some common macros.
// NB. we only use macros when no alternatives.
//===--------------------------------------===

#pragma once

#undef DISALLOW_COPY
#define DISALLOW_COPY(classname)                                               \
  classname(const classname &) = delete;                                       \
  auto operator=(const classname &)->classname & = delete;                     \
  classname(classname &) = delete;                                             \
  auto operator=(classname &)->classname & = delete

#undef DISALLOW_MOVE
#define DISALLOW_MOVE(classname)                                               \
  classname(classname &&) = delete;                                            \
  auto operator=(classname &&)->classname && = delete

#undef DISALLOW_COPY_AND_MOVE
#define DISALLOW_COPY_AND_MOVE(classname)                                      \
  DISALLOW_COPY(classname);                                                    \
  DISALLOW_MOVE(classname)
