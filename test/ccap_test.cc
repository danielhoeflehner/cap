#include "../ccap.h"

#include <gtest/gtest.h>

TEST(Arguments, BasicTest)
{
  ccap::Argument arg = ccap::Argument::WithName("test");
  EXPECT_EQ("test", arg.GetName());
}