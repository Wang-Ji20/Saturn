//===------------------------------------------===
// Saturn 2023
//
// Identification: test/port/unix_file_system_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "port/local_file_handle.hh"
#include "port/local_file_system.hh"
#include "gtest/gtest.h"

TEST(FSTest, LocalFS) {
  using namespace saturn;
  using namespace std;

  auto fs = make_unique<LocalFileSystem>();
  auto handle =
      fs->Open("./test.test",
               OpenFlags::WRITE | OpenFlags::READ | OpenFlags::CREATE_TRUNCATE);
  ASSERT_TRUE(handle);
  auto size = fs->GetFileSize(*handle);
  if (!size.ok()) {
    ASSERT_TRUE(false);
  }
  ASSERT_EQ(*size, 0);
  auto type = fs->GetFileType(*handle);
  ASSERT_EQ(type, FileType::REGULAR);
  auto written = fs->Write(*handle, "hello", 5ULL);
  fs->Reset(*handle);
  auto pos = fs->GetPosition(*handle);
  ASSERT_EQ(pos, 0);
  fs->Seek(*handle, 0ULL);
  handle->Close();
  fs->Remove("./test.test");
}
