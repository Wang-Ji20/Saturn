//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/storage/storage_magic.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/storage_magic.hh"
#include "common/exception.hh"

namespace saturn {

auto CemeteryOfInnocent::SaturnReadValue(Deserializer &deserializer)
    -> CemeteryOfInnocent {
  deserializer.OnObjectBegin();
  string magic_string;
  deserializer.ReadProperty("magic_string", magic_string);
  u64 version;
  deserializer.ReadProperty("version", version);
  deserializer.OnObjectEnd();
  if (magic_string != kMagicString) {
    throw IllegalArgumentException("you are not using a saturn database file");
  }
  return CemeteryOfInnocent{.version = Version(version)};
}

} // namespace saturn
