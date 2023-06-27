//===------------------------------------------===
// Saturn 2023
//
// Identification: lib\common\assert.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/assert.hh"
#include <iostream>

namespace saturn {

void SaturnAssert(bool condition, std::string_view message, std::string_view file, int line) noexcept {
    if (condition) {
        return;
    }
    std::cerr << "Assertion failed: " << message << " at " << file << ":" << line << std::endl;
    std::abort();
}

} // namespace saturn