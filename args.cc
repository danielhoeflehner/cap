#include "args.h"

#include <iostream>
#include <optional>
#include <string>

#include "argument.h"

Args::Args(int argc, char const *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    raw_args_.push_back(argv[i]);
  }

  num_args_ = argc - 1;
}

Args Args::From(int argc, char const *argv[])
{
  return Args(argc, argv);
}

auto Args::Arg(Argument item) -> Args &
{
  args_.push_back(item);
  return *this;
}

auto Args::GetMatches() -> Args &
{
  for (int i = 0; i < num_args_; ++i)
  {
    if (raw_args_[i].starts_with("--"))
    {
      std::cout << raw_args_[i] << std::endl;
    }
    else
    {
      args_[i].SetValue(raw_args_[i]);
    }
  }

  return *this;
}

auto Args::Get(std::string arg_name) const -> std::optional<std::string>
{
  for (const auto &argument : args_)
  {
    if (argument.GetName() == arg_name) {
      return argument.GetValue();
    }
  }

  return std::nullopt;
}