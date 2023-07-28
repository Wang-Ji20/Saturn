//===------------------------------------------===
// Saturn 2023
//
// Identification: test/storage/file_buffer_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/unique_ptr.hh"
#include "port/local_file_system.hh"
#include "storage/allocator.hh"
#include "storage/file_buffer.hh"
#include "common/constant.hh"

#include "gtest/gtest.h"

TEST(FileBufferTest, ReadWrite) {
  using namespace saturn;
  using namespace std;

  auto fs = make_unique<LocalFileSystem>();
  auto& alloc = Allocator::GetDefaultAllocator();
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

  auto buf = make_unique<FileBuffer>(alloc, FileBufferType::BLOCK, 128ULL);
  ASSERT_EQ(buf->limitSize, Storage::SECTOR_SIZE - Storage::BLOCK_HEADER_SIZE);
  buf->buffer[0] = 'a';
  buf->buffer[1] = 'b';
  buf->buffer[2] = 'c';
  buf->Write(*handle, 0ULL);

  fs->Reset(*handle);
  auto pos = fs->GetPosition(*handle);
  ASSERT_EQ(pos, 0);
  fs->Seek(*handle, 0ULL);
  handle->Close();
  fs->Remove("./test.test");
}

