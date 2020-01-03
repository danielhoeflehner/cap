#ifndef GRRS_ARGS_H_
#define GRRS_ARGS_H_

#include "argument.h"

#include <string>
#include <optional>
#include <vector>

class Args
{
private:
  std::vector<std::string> raw_args_;
  std::vector<Argument> args_;
  int num_args_;

public:
  Args() = delete;
  Args(int argc, char const *argv[]);
  static Args From(int argc, char const *argv[]);
  auto Arg(Argument item) -> Args &;
  auto GetMatches() -> Args &;
  auto Get(std::string arg_name) const -> std::optional<std::string>;
};

#endif