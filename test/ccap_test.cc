#include "../ccap.h"

#include <gtest/gtest.h>

using namespace ccap;

TEST(ArgumentTest, ItShouldBeInitializedWithAName)
{
  Argument arg1 = Argument("argument1");
  EXPECT_EQ("argument1", arg1.GetName());

  Argument arg2 = Argument::WithName("argument2");
  EXPECT_EQ("argument2", arg2.GetName());
}