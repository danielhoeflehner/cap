#include "../ccap.h"

#include <gtest/gtest.h>

using namespace ccap;

TEST(FlagTest, DefaultsWithConstructor) {
  Flag f = Flag("flag");
  EXPECT_EQ("flag", f.GetName());
  EXPECT_EQ('f', f.GetShort());
  EXPECT_EQ("flag", f.GetLong());
  EXPECT_TRUE(f.IsOption());
  EXPECT_FALSE(f.IsGiven());
}

TEST(FlagTest, DefaultsFromStatic) {
  Flag f = Flag::WithName("name");
  EXPECT_EQ("name", f.GetName());
  EXPECT_EQ('n', f.GetShort());
  EXPECT_EQ("name", f.GetLong());
  EXPECT_TRUE(f.IsOption());
  EXPECT_FALSE(f.IsGiven());
}

TEST(FlagTest, InitWithSpecificShort) {
  Flag f = Flag("Flag1");
  f.SetShort('t');
  EXPECT_EQ('t', f.GetShort());
}

TEST(FlagTest, InitWithSpecificLong) {
  Flag f = Flag("Flag1");
  f.SetLong("test");
  EXPECT_EQ("test", f.GetLong());
}

TEST(FlagTest, InitWithDefaultValue) {
  Flag f = Flag("Flag");
  f.SetValue("test_value");
  EXPECT_EQ("test_value", f.GetValue());
}

TEST(FlagTest, ExpectsAndRequired) {
  Flag f = Flag("Flag");
  EXPECT_FALSE(f.IsExpectingValue());
  EXPECT_FALSE(f.IsRequired());

  f.ExpectsValue();
  f.Required();
  EXPECT_TRUE(f.IsExpectingValue());
  EXPECT_TRUE(f.IsRequired());
}

TEST(ArgsTest, Initialization) {
  int argc = 2;
  const char* argv[] = {"program", "Flag1"};

  {
    Args args = Args(argc, argv);
    args.Parse();

    EXPECT_EQ(std::nullopt, args.Get("Flag1"));
    EXPECT_EQ(std::nullopt, args.Get("does_not_exist"));
  }

  {
    Args args = Args::From(argc, argv);
    args.Parse();

    EXPECT_EQ(std::nullopt, args.Get("Flag1"));
    EXPECT_EQ(std::nullopt, args.Get("does_not_exist"));
  }
}

/* TEST(ArgsTest, ItShouldShowHelp)
{
  int argc = 2;
  const char* argv[] = {"program", "--help"};

  {
    Args args = Args(argc, argv);
    args.SetAbout("Test Programm")
        .SetAuthor("Its a me")
        .SetName("App")
        .SetVersion("0.0.2")
        .Parse();
  }
} */

TEST(ArgsTest, RequiredFlag) {
  {
    const char* argv[] = {"ccap", "-n", "John"};

    Args args = Args(3, argv);
    args.Arg(Flag::WithName("name").Required().ExpectsValue());
    args.Parse();

    EXPECT_TRUE(args.Get("name").has_value());
    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap", "--name", "John"};

    Args args = Args(3, argv);
    args.Arg(Flag::WithName("name").Required().ExpectsValue());
    args.Parse();

    EXPECT_TRUE(args.Get("name").has_value());
    EXPECT_EQ("John", args.Get("name").value());
  }

  {
    const char* argv[] = {"ccap", "--other", "John"};

    Args args = Args(1, argv);
    args.Arg(Flag::WithName("name").Required().ExpectsValue());

    EXPECT_EXIT(args.Parse(), ::testing::ExitedWithCode(EXIT_FAILURE), "");
  }
}

TEST(ArgsTest, ExpectsValue) {
  {
    const char* argv[] = {"ccap", "--name"};

    Args args = Args(2, argv);
    args.Arg(Flag::WithName("name").Required().ExpectsValue());

    EXPECT_EXIT(args.Parse(), ::testing::ExitedWithCode(EXIT_FAILURE), "");
  }

  {
    const char* argv[] = {"ccap", "-n"};

    Args args = Args(2, argv);
    args.Arg(Flag::WithName("name").Required().ExpectsValue());

    EXPECT_EXIT(args.Parse(), ::testing::ExitedWithCode(EXIT_FAILURE), "");
  }

  {
    const char* argv[] = {"ccap", "-n"};

    Args args = Args(2, argv);
    args.Arg(Flag::WithName("name").ExpectsValue());
    args.Parse();

    EXPECT_FALSE(args.Get("name").has_value());
  }
}

TEST(ArgsTest, GetValueWithShortOption) {
  const char* argv[] = {"ccap", "-n", "John"};

  Args args = Args(3, argv);
  args.Arg(Flag::WithName("name").ExpectsValue());
  args.Parse();

  EXPECT_TRUE(args.Get("name").has_value());
  EXPECT_EQ("John", args.Get("name").value());
}

TEST(ArgsTest, GetValueWithLongOption) {
  const char* argv[] = {"ccap", "--name", "John"};

  Args args = Args(3, argv);
  args.Arg(Flag::WithName("name").SetLong("name").ExpectsValue());
  args.Parse();

  EXPECT_TRUE(args.Get("name").has_value());
  EXPECT_EQ("John", args.Get("name").value());
}

TEST(ArgsTest, GetNoValueWhenFlagIsNotGiven) {
  const char* argv[] = {"ccap"};

  Args args = Args(1, argv);
  args.Arg(Flag::WithName("name").ExpectsValue());
  args.Parse();

  EXPECT_FALSE(args.Get("name").has_value());
}

TEST(ArgsTest, GetNoValueWhenFlagIsNotProperlyGiven) {
  const char* argv[] = {"ccap", "-"};

  Args args = Args(2, argv);
  args.Arg(Flag::WithName("name").ExpectsValue());
  args.Parse();

  EXPECT_FALSE(args.Get("name").has_value());
}

TEST(ArgsTest, BooleanFlag) {
  {
    const char* argv[] = {"ccap", "-v"};

    Args args = Args(2, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
  }

  {
    const char* argv[] = {"ccap", "--verbose"};

    Args args = Args(2, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
  }

  {
    const char* argv[] = {"ccap", "-v", "-c", "-o"};

    Args args = Args(4, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Arg(Flag::WithName("compile"));
    args.Arg(Flag::WithName("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "--verbose", "--compile", "--output"};

    Args args = Args(4, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Arg(Flag::WithName("compile"));
    args.Arg(Flag::WithName("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "-v", "--compile", "-o"};

    Args args = Args(4, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Arg(Flag::WithName("compile"));
    args.Arg(Flag::WithName("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_TRUE(args.IsGiven("output"));
  }

  {
    const char* argv[] = {"ccap", "-v", "--compile"};

    Args args = Args(3, argv);
    args.Arg(Flag::WithName("verbose"));
    args.Arg(Flag::WithName("compile"));
    args.Arg(Flag::WithName("output"));
    args.Parse();

    EXPECT_TRUE(args.IsGiven("verbose"));
    EXPECT_TRUE(args.IsGiven("compile"));
    EXPECT_FALSE(args.IsGiven("output"));
  }
}