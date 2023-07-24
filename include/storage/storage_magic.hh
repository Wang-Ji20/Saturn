//===------------------------------------------===
// Saturn 2023
//
// Identification: include/storage/storage_magic.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

//===------------------------------------------------===
// This file contains magic string/number in storage module.
//===------------------------------------------------===

#pragma once

#include "common/constant.hh"
#include "common/serializer/deserializer.hh"
#include "common/type.hh"

namespace saturn {

// The first header of all database files.
// size: 4088 bytes
// first 6 bytes: "saturn"
// next 8 bytes: version
// other reserved
struct CemeteryOfInnocent {
  static constexpr const char *kMagicString = "saturn";
  static constexpr Size kMagicStringLength = 6_Size;
  Version version = Storage::VERSION;

  // throw exception if not match, because it is severe and we can stop a user
  // session
  static void CheckMagicString();

  template <typename Ser>
  friend void SaturnWriteValue(Ser &serializer,
                               const CemeteryOfInnocent &value) {
    serializer.OnObjectBegin();
    serializer.SetTag("magic_string");
    serializer.WriteValue(kMagicString);
    serializer.SetTag("version");
    serializer.WriteValue(static_cast<u64>(value.version));
    serializer.OnObjectEnd();
  }

  static auto SaturnReadValue(Deserializer &deserializer) -> CemeteryOfInnocent;
};

} // namespace saturn
