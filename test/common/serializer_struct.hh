//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/serializer_struct.hh
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===
#pragma once

#include "common/serializer/deserializer.hh"
#include "common/serializer/serializer.hh"

struct SomeComplexStruct {
  int x;
  int y;

  template <typename Ser>
  friend void SaturnWriteValue(Ser &serializer,
                               const SomeComplexStruct &value) {
    serializer.OnObjectBegin();
    serializer.SetTag("x");
    serializer.WriteValue(value.x);
    serializer.SetTag("y");
    serializer.WriteValue(value.y);
    serializer.OnObjectEnd();
  }

  static auto SaturnReadValue(saturn::Deserializer &deserializer)
      -> SomeComplexStruct {
    deserializer.OnObjectBegin();
    int deserialize_x;
    int deserialize_y;
    deserializer.ReadProperty("x", deserialize_x);
    deserializer.ReadProperty("y", deserialize_y);
    deserializer.OnObjectEnd();
    return SomeComplexStruct{deserialize_x, deserialize_y};
  }
};
