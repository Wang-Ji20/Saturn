//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/logger.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// this file contains the logger & checker for Saturn.
//===------------------------------------------------===

#pragma once

#include "absl/log/check.h"
#include "absl/log/log.h"

#define SaturnUnreachable()                                                    \
  do {                                                                         \
    CHECK(FATAL) << "Unreachable code reached.";                               \
    __builtin_unreachable();                                                   \
  } while (0)
