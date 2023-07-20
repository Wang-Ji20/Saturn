//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/exception_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "absl/strings/str_format.h"
#include "gtest/gtest.h"

#define format(...) absl::StrFormat(__VA_ARGS__)

TEST(FormatTest, Basic) {
  constexpr absl::string_view msg = "hello world %s";
  ASSERT_EQ(absl::StrFormat(msg, "hello world"), "hello world hello world");
  ASSERT_EQ(format("hello %s %d", "world", 2023), "hello world 2023");
  ASSERT_EQ(format("hello %s %d %s", "world", 2023, "Saturn"), "hello world 2023 Saturn");
}
