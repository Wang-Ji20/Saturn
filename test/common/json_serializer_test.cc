//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/json_serializer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/json_deserializer.hh"
#include "common/serializer/json_serializer.hh"
#include "serializer_struct.hh"
#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <utility>

TEST(JsonSerializerTest, Basic) {
  using namespace saturn;
  auto p = std::make_pair("hello", "world");
  auto result = JsonSerializer::ToString(p);
  ASSERT_EQ(result,
            "[\n    {\n        \"key\": \"hello\",\n        \"value\": "
            "\"world\"\n    }\n]");
}

TEST(JsonSerializerTest, Extension) {
  using namespace saturn;
  SomeComplexStruct scs{1, 2};
  auto result = JsonSerializer::ToString(scs);
  ASSERT_EQ(result, "[\n    {\n        \"x\": 1,\n        \"y\": 2\n    }\n]");
}

TEST(JsonDeserializerTest, Basic) {
  using namespace saturn;
  string s1 = "hello";
  string s2 = "world";
  auto p = std::make_pair(s1, s2);
  auto result = JsonSerializer::ToString(p);
  auto deserializer = JsonDeserializer::FromString(result);
  auto deserialize_p = deserializer.Read<std::pair<std::string, std::string>>();
  ASSERT_EQ(deserialize_p, p);
}

TEST(JsonDeserializerTest, Extension) {
  using namespace saturn;
  string str = "[\n {\n        \"x\": 1,\n        \"y\": 2\n    }\n ]";
  auto deserializer = JsonDeserializer::FromString(str);
  static_assert(has_deserialize<SomeComplexStruct>(), "nono");
  auto result = deserializer.Read<unique_ptr<SomeComplexStruct>>();
  ASSERT_EQ(result->x, 1);
  ASSERT_EQ(result->y, 2);
}
