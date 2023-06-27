//===------------------------------------------===
// Saturn 2023
//
// Identification: lib\include\common\assert.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include <cassert>
#include <string_view>

namespace saturn {
void SaturnAssert(bool condition, std::string_view message, std::string_view file, int line) noexcept;
} // namespace saturn

#define SATURN_ASSERT(condition, message) \
    saturn::SaturnAssert(condition, message, __FILE__, __LINE__)