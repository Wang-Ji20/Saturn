//===------------------------------------------===
// Saturn 2023
//
// Identification: include\common\format.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "absl/strings/str_format.h"

#define format(...) absl::StrFormat(__VA_ARGS__)
