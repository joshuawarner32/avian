#include "toy.h"
#include <test-harness.h>

using namespace avian::codegen;
using namespace avian::util;

TEST(Constants)
{
  Machine m;
  assertEqual(1.0, m.line("1").get());
  assertEqual(42.0, m.line("42").get());
  assertEqual(42.0, m.line("42.0").get());
}

TEST(Constant_Add)
{
  Machine m;
  assertEqual(3.0, m.line("1+2").get());
  assertEqual(3.0, m.line("1 + 2").get());
}

TEST(Constant_Mul)
{
  Machine m;
  assertEqual(6.0, m.line("3*2").get());
  assertEqual(6.0, m.line("3 * 2").get());
}

TEST(Constant_Sub)
{
  Machine m;
  assertEqual(1.0, m.line("3-2").get());
  assertEqual(-1.0, m.line("1 - 2").get());
}

TEST(Constant_LessThan)
{
  Machine m;
  assertEqual(1.0, m.line("1<2").get());
  assertEqual(0.0, m.line("2<1").get());
  // assertEqual(1.0, m.line("-2<-1").get());
  // assertEqual(0.0, m.line("10<-12.0").get());
}

TEST(Constant_If)
{
  Machine m;
  assertEqual(3.0, m.line("if 0 then 2 else 3").get());
  assertEqual(2.0, m.line("if 1 then 2 else 3").get());
}

TEST(Constant_For)
{
  Machine m;
  assertEqual(0.0, m.line("for i = 0, i < 2 in 1").get());
  // assertEqual(0.0, m.line("for i = 0, i < 2, 3.0 in 1").get());
}

TEST(Functions_Simple)
{
  Machine m;
  assertFalse(m.line("def inc(x) x + 1;").isPresent());
  assertEqual(3.0, m.line("inc(2)").get());
}
