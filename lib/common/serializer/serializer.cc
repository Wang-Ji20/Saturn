//===------------------------------------------===
// Saturn 2023
//
// Identification: lib/common/serializer/serializer.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/serializer.hh"

namespace saturn {

void Serializer::WriteValue(const string &value) {
  OnObjectBegin();
  buffer.insert(buffer.end(), value.begin(), value.end());
  OnObjectEnd();
}

void Serializer::WriteValue(const char *value) {
  OnObjectBegin();
  buffer.insert(buffer.end(), value, value + strlen(value));
  OnObjectEnd();
}

} // namespace saturn
