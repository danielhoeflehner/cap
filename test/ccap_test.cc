#include "../ccap.h"

#include <gtest/gtest.h>

using namespace ccap;

TEST(ArgumentTest, Initialization)
{
  {
    Argument arg1 = Argument("argument1");
    EXPECT_EQ("argument1", arg1.GetName());
  }

  {
    Argument arg2 = Argument::WithName("argument2");
    EXPECT_EQ("argument2", arg2.GetName());
  }

  {
    Argument arg = Argument("argument1");
    arg.SetShort('t');
    EXPECT_EQ('t', arg.GetShort());
  }

  {
    Argument arg = Argument("argument1");
    arg.SetLong("test");
    EXPECT_EQ("test", arg.GetLong());
  }

  {
    Argument arg = Argument("argument");
    arg.SetValue("test_value");
    EXPECT_EQ("test_value", arg.GetValue());
  }

  {
    Argument arg = Argument("argument");
    EXPECT_FALSE(arg.IsExpectingValue());
    EXPECT_FALSE(arg.IsRequired());

    arg.ExpectsValue();
    arg.Required();
    EXPECT_TRUE(arg.IsExpectingValue());
    EXPECT_TRUE(arg.IsRequired());
  }
}

TEST(ArgsTest, Initialization)
{
  int argc = 2;
  const char* argv[] = {"program", "argument1"};

  {
    Args args = Args(argc, argv);
    EXPECT_EQ(1, args.GetRawArgsCount());
    EXPECT_EQ("argument1", args.GetRawArgs().at(0));
  }

  {
    Args args = Args::From(argc, argv);
    EXPECT_EQ(1, args.GetRawArgsCount());
    EXPECT_EQ("argument1", args.GetRawArgs().at(0));
  }

  {
    Args args = Args(0, argv);
    EXPECT_EQ(0, args.GetRawArgsCount());
    EXPECT_THROW(args.GetRawArgs().at(0), std::out_of_range);
  }
}