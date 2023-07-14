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
};

TEST(JsonSerializerTest, Basic) {
  using namespace saturn;
  auto p = std::make_pair("hello", "world");
  auto result = JsonSerializer::ToString(p);
  std::cerr << result << '\n';
  ASSERT_EQ(result, "[\n    {\n        \"key\": \"hello\",\n        \"value\": \"world\"\n    }\n]");
}
