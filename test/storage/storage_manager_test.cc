//===------------------------------------------===
// Saturn 2023
//
// Identification: test/storage/storage_manager_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/serializer/json_serializer.hh"
#include "common/serializer/json_deserializer.hh"
#include "storage/single_file_storage_manager.hh"
#include "storage/storage_magic.hh"
#include "gtest/gtest.h"

TEST(StorageManagerTest, Basic) {
  using namespace saturn;
  using namespace std;
  // TODO JUST TODO
  ASSERT_TRUE(true);
}

TEST(StorageTest, CDIHeader) {
  using namespace saturn;
  CemeteryOfInnocent cdi;
  auto strCdi = JsonSerializer::ToString(cdi);
  std::cerr << strCdi << std::endl;
  ASSERT_EQ(strCdi,
            "[\n    {\n        \"magic_string\": \"saturn\",\n        "
            "\"version\": 0\n    }\n]");
}

TEST(StorageTest, CDIHeaderDeserialize) {
  using namespace saturn;
  using namespace std;
  CemeteryOfInnocent cdi;
  auto strCdi = JsonSerializer::ToString(cdi);
  CemeteryOfInnocent result = JsonDeserializer::FromString(strCdi).Read<CemeteryOfInnocent>();
  ASSERT_EQ(result.version, cdi.version);
}
