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

struct SomeComplexStruct {
  int x;
  int y;
};

TEST(JsonSerializerTest, DISABLED_Basic) {
  using namespace saturn;
  auto p = std::make_pair("hello", "world");
  auto result = JsonSerializer::ToString(p);
  EXPECT_EQ(result, "\"hello\"\n");
}
