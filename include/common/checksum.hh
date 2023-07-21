//===------------------------------------------===
// Saturn 2023
//
// Identification: include/common/checksum.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#pragma once

#include "common/type.hh"

namespace saturn {

struct HashingConstants {
  static constexpr u64 kMagicOne = 0x9e3779b97f4a7c15ULL;
  static constexpr u64 kMagicTwo = 0x9e3779b97f4a7c15ULL;
};

/// This struct provides some hashing utilities.
/// for example checksum, hash, etc.
struct HashingUtilities {

  static auto Checksum(u64 data) -> u64 {
    return data * HashingConstants::kMagicOne;
  }

  static auto Checksum(Datum *data, Size size) -> u64 {
    Size inEightBytes = Size(size / sizeof(u64));
    u64 *data64 = reinterpret_cast<u64 *>(data);
    u64 checksum = 0;
    for (auto i = 0_Size; i < inEightBytes; ++i) {
      checksum ^= Checksum(data64[i]);
    }
    for (auto i = inEightBytes * sizeof(u64); i < size; ++i) {
      checksum ^= Checksum(data[i]);
    }
    return checksum;
  }

};

} // namespace saturn
