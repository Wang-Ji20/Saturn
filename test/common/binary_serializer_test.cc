//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/binary_serializer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/binary_deserializer.hh"
#include "common/serializer/binary_serializer.hh"
#include "serializer_struct.hh"
#include "gtest/gtest.h"

#include <iostream>

TEST(BinarySerializerTest, Basic) {
  using namespace saturn;
  auto p = std::make_pair("hello", "world");
  auto result = BinarySerializer::Serialize(p);
  vector<Datum> expectedResult = {
      '\5',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',  '\x68',
      '\x65', '\x6c', '\x6c', '\x6f', '\5',   '\0',   '\0',   '\0',  '\0',
      '\0',   '\0',   '\0',   '\x77', '\x6f', '\x72', '\x6c', '\x64'};
  ASSERT_EQ(result, expectedResult);
}

TEST(BinarySerializerTest, Extension) {
  using namespace saturn;
  SomeComplexStruct scs{1, 2};
  auto result = BinarySerializer::Serialize(scs);
  vector<Datum> expectedResult = {'\2', '\0', '\0', '\0', '\x14', '\0', '\0',
                                  '\0', '\0', '\0', '\0', '\0',   '\1', '\0',
                                  '\0', '\0', '\2', '\0', '\0',   '\0'};
  ASSERT_EQ(result, expectedResult);
}

TEST(BinaryDeserializerTest, Basic) {
  using namespace saturn;
  vector<Datum> data = {'\5',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
                        '\0',   '\x68', '\x65', '\x6c', '\x6c', '\x6f', '\5',
                        '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
                        '\x77', '\x6f', '\x72', '\x6c', '\x64'};
  auto result =
      BinaryDeserializer::Deserialize<std::pair<string, string>>(
          data.data(), Size(data.size()));
  ASSERT_EQ(result->first, "hello");
  ASSERT_EQ(result->second, "world");
}

TEST(BinaryDeserializerTest, Extension) {
  using namespace saturn;
  vector<Datum> data = {'\2', '\0', '\0', '\0', '\x14', '\0', '\0', '\0', '\0',
                        '\0', '\0', '\0', '\1', '\0', '\0', '\0', '\2', '\0',
                        '\0', '\0'};
  auto result =
      BinaryDeserializer::Deserialize<SomeComplexStruct>(
          data.data(), Size(data.size()));
  ASSERT_EQ(result->x, 1);
  ASSERT_EQ(result->y, 2);
}
