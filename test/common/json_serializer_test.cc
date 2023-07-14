//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/json_serializer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/json_serializer.hh"
#include "gtest/gtest.h"

#include <string>
#include <utility>
#include <iostream>

struct SomeComplexStruct {
  int x;
  int y;

  template <typename Ser>
  friend void SaturnWriteValue(Ser& serializer, const SomeComplexStruct& value) {
    serializer.OnObjectBegin();
    serializer.SetTag("x");
    serializer.WriteValue(value.x);
    serializer.SetTag("y");
    serializer.WriteValue(value.y);
    serializer.OnObjectEnd();
  }
};

TEST(JsonSerializerTest, Basic) {
  using namespace saturn;
  auto p = std::make_pair("hello", "world");
  auto result = JsonSerializer::ToString(p);
  ASSERT_EQ(result, "[\n    {\n        \"key\": \"hello\",\n        \"value\": \"world\"\n    }\n]");
}

TEST(JsonSerializerTest, Extension) {
  using namespace saturn;
  SomeComplexStruct scs {1, 2};
  auto result = JsonSerializer::ToString(scs);
  ASSERT_EQ(result, "[\n    {\n        \"x\": 1,\n        \"y\": 2\n    }\n]");
}
