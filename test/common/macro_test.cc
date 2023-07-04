//===------------------------------------------===
// Saturn 2023
//
// Identification: test/common/macro_test.cc
//
// Author: Ji Wang <jiwangcdi@gmail.com>
//
//===------------------------------------------===

#include "common/macro.hh"

#include "gtest/gtest.h"

//===------------------------------------------------===
// some classes for test
//===------------------------------------------------===

class Base {
public:
  Base() = default;
  virtual ~Base() = default;
  CAST_TO_DERIVED(Base);
  int a {3};
};

class Derived : public Base {
public:
  Derived() = default;
  ~Derived() final = default;
};

class NotDerived {
public:
  NotDerived() = default;
  ~NotDerived() = default;
};

TEST(MacroTest, CastTest) {
    Base b;
    auto& d = b.Cast<Derived>();
    ASSERT_EQ(d.a, 3);
}

SATURN_ENUM_CLASS(TestEnum, int) {
  aaa = 1 << 0, 
  bbb = 1 << 1,
  ccc = 1 << 2,
  ddd = 1 << 3,
};

TEST(MacroTest, EnumClassTest) {
  auto abc = TestEnum::aaa | TestEnum::bbb | TestEnum::ccc;
  auto a = TestEnum::aaa;
  ASSERT_TRUE(ContainFlags(abc, a));
  ASSERT_FALSE(ContainFlags(abc, TestEnum::ddd));
  ASSERT_TRUE(ContainFlags(abc, TestEnum::aaa));
  ASSERT_TRUE(ContainFlags(abc, TestEnum::bbb));
  ASSERT_TRUE(ContainFlags(abc, TestEnum::ccc));
  ASSERT_TRUE(ContainFlags(abc, TestEnum::ddd, TestEnum::aaa));
  ASSERT_TRUE(ContainFlags(abc, TestEnum::ddd, TestEnum::bbb));
  ASSERT_TRUE(ContainFlags(a, TestEnum::ddd, TestEnum::ccc, TestEnum::bbb, TestEnum::aaa));
}
