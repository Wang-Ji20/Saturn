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

auto DatabaseHeader::SaturnReadValue(Deserializer &deserializer)
    -> DatabaseHeader {
  deserializer.OnObjectBegin();
  DatabaseHeader ret;
  deserializer.ReadProperty("iteration", ret.iteration);
  u64 rootMeta;
  deserializer.ReadProperty("rootMeta", rootMeta);
  ret.rootMeta = BlockId(rootMeta);
  u64 freeList;
  deserializer.ReadProperty("freeList", freeList);
  ret.freeList = BlockId(freeList);
  deserializer.ReadProperty("blockCount", ret.blockCount);
  return ret;
}

} // namespace saturn
