#include "../ccap.h"

#include <gtest/gtest.h>

using namespace ccap;

TEST(ArgumentTest, Initialization)
{
  {
    Argument arg1 = Argument("argument1");
    EXPECT_EQ("argument1", arg1.GetName());
    EXPECT_TRUE(arg1.IsOption());
    EXPECT_FALSE(arg1.IsGiven());
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
    args.Parse();

    EXPECT_EQ(std::nullopt, args.Get("argument1"));
    EXPECT_EQ(std::nullopt, args.Get("does_not_exist"));
  }

  {
    Args args = Args::From(argc, argv);
    args.Parse();

    EXPECT_EQ(std::nullopt, args.Get("argument1"));
    EXPECT_EQ(std::nullopt, args.Get("does_not_exist"));
  }
}

TEST(ArgsTest, ItShouldTakeAnRequiredOption)
{
  {
    const char* argv[] = {"ccap", "-n", "John"};

    Args args = Args(3, argv);
    args.Arg(
        Argument::WithName("name").SetShort('n').Required().ExpectsValue());
    args.Parse();

    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap", "-n", "      John"};

    Args args = Args(3, argv);
    args.Arg(
        Argument::WithName("name").SetShort('n').Required().ExpectsValue());
    args.Parse();

    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap", "--name", "   John      "};

    Args args = Args(3, argv);
    args.Arg(
        Argument::WithName("name").SetLong("name").Required().ExpectsValue());
    args.Parse();

    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap"};

    Args args = Args(1, argv);
    args.Arg(
        Argument::WithName("name").SetShort('n').Required().ExpectsValue());
    EXPECT_EXIT(args.Parse(), ::testing::ExitedWithCode(EXIT_FAILURE), "");
  }
}

TEST(ArgsTest, ItShouldTakeAnOptionalOption)
{
  {
    const char* argv[] = {"ccap", "-n", "John"};

    Args args = Args(3, argv);
    args.Arg(Argument::WithName("name").SetShort('n').ExpectsValue());
    args.Parse();

    EXPECT_TRUE(args.Get("name").has_value());
    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap", "--name", "John"};

    Args args = Args(3, argv);
    args.Arg(Argument::WithName("name").SetLong("name").ExpectsValue());
    args.Parse();

    EXPECT_TRUE(args.Get("name").has_value());
    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap"};

    Args args = Args(1, argv);
    args.Arg(Argument::WithName("name").SetShort('n').ExpectsValue());
    args.Parse();

    EXPECT_FALSE(args.Get("name").has_value());
  }
}

TEST(ArgsTest, ItShouldTakeAFlag)
{
  {
    const char* argv[] = {"ccap", "-v"};

    Args args = Args(2, argv);
    args.Arg(Argument::WithName("verbose").SetShort('v'));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
  }

  {
    const char* argv[] = {"ccap", "-v", "-c", "-o"};

    Args args = Args(4, argv);
    args.Arg(Argument::WithName("verbose").SetShort('v'));
    args.Arg(Argument::WithName("compile").SetShort('c'));
    args.Arg(Argument::WithName("output").SetShort('o'));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "--verbose"};

    Args args = Args(2, argv);
    args.Arg(Argument::WithName("verbose").SetLong("verbose"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
  }

  {
    const char* argv[] = {"ccap", "--verbose", "--compile", "--output"};

    Args args = Args(4, argv);
    args.Arg(Argument::WithName("verbose").SetShort('v').SetLong("verbose"));
    args.Arg(Argument::WithName("compile").SetShort('c').SetLong("compile"));
    args.Arg(Argument::WithName("output").SetShort('o').SetLong("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "-v", "--compile", "-o"};

    Args args = Args(4, argv);
    args.Arg(Argument::WithName("verbose").SetShort('v').SetLong("verbose"));
    args.Arg(Argument::WithName("compile").SetShort('c').SetLong("compile"));
    args.Arg(Argument::WithName("output").SetShort('o').SetLong("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "-v", "--compile"};

    Args args = Args(3, argv);
    args.Arg(Argument::WithName("verbose").SetShort('v').SetLong("verbose"));
    args.Arg(Argument::WithName("compile").SetShort('c').SetLong("compile"));
    args.Arg(Argument::WithName("output").SetShort('o').SetLong("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_FALSE(args.IsGiven("output"));
  }
}

TEST(HelperFunctions, Trim)
{
  {
    std::string s{"/path/to/file     "};
    Trim(s);
    EXPECT_EQ("/path/to/file", s);
  }

  {
    std::string s{"      /path/to/file"};
    Trim(s);
    EXPECT_EQ("/path/to/file", s);
  }

  {
    std::string s{" /path/to/file     "};
    Trim(s);
    EXPECT_EQ("/path/to/file", s);
  }
}