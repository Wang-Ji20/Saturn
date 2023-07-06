//===------------------------------------------===
// Saturn 2023
//
// Identification: test/storage/storage_manager_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "storage/storage_manager.hh"
#include "gtest/gtest.h"

TEST(StorageManagerTest, Basic) {
  using namespace saturn;
  using namespace std;

  auto sm = make_unique<StorageManager>("./test");
  ASSERT_TRUE(sm);
}
