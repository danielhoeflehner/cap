#ifndef CCAP_CCAP_H_
#define CCAP_CCAP_H_

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace ccap {

class Argument {
 private:
  bool required_ = false;
  std::string long_;
  std::string name_;
  char short_;
  std::string value_;

 public:
  Argument() = delete;
  Argument(std::string name);
  static auto WithName(std::string name) -> Argument;
  auto GetShort() const -> char;
  auto SetShort(char s) -> Argument &;
  auto GetLong() const -> std::string;
  auto SetLong(std::string l) -> Argument &;
  auto SetValue(std::string value) -> Argument &;
  auto GetValue() const -> std::optional<std::string>;
  auto GetName() const -> std::string;
  auto Required() -> Argument &;
  auto IsRequired() const -> bool;
};

Argument::Argument(std::string name) : name_(name){};

auto Argument::WithName(std::string name) -> Argument { return Argument(name); }

auto Argument::GetShort() const -> char { return short_; }

auto Argument::SetShort(char s) -> Argument & {
  short_ = s;
  return *this;
}

auto Argument::GetLong() const -> std::string { return long_; }

auto Argument::SetLong(std::string l) -> Argument & {
  long_ = l;
  return *this;
}

auto Argument::SetValue(std::string value) -> Argument & {
  value_ = value;
  return *this;
}

auto Argument::GetValue() const -> std::optional<std::string> {
  if (value_.size() > 0) {
    return value_;
  }

  return std::nullopt;
}

auto Argument::GetName() const -> std::string { return name_; }

auto Argument::Required() -> Argument & {
  required_ = true;
  return *this;
}

auto Argument::IsRequired() const -> bool { return required_; }

class Args {
 private:
  std::vector<std::string> raw_args_;
  std::vector<Argument> args_;
  int num_args_;

 public:
  Args() = delete;
  Args(int argc, char const *argv[]);
  static auto From(int argc, char const *argv[]) -> Args;
  auto Arg(Argument item) -> Args &;
  auto Parse() -> Args &;
  auto Get(std::string arg_name) const -> std::optional<std::string>;
};

Args::Args(int argc, char const *argv[]) {
  for (int i = 1; i < argc; ++i) {
    raw_args_.push_back(argv[i]);
  }

  num_args_ = argc - 1;
}

auto Args::From(int argc, char const *argv[]) -> Args {
  return Args(argc, argv);
}

auto Args::Arg(Argument item) -> Args & {
  args_.push_back(item);
  return *this;
}

auto Args::Parse() -> Args & {
  for (int i = 0; i < num_args_; ++i) {
    if (raw_args_[i].starts_with("--")) {
      std::string name = raw_args_[i].substr(2);
      std::string v = raw_args_[i + 1];
      for (auto &arg : args_) {
        if (arg.GetLong() == name) {
          arg.SetValue(v);
        }
      }
    } else if (raw_args_[i].starts_with("-")) {
      char shortName = raw_args_[i][1];
      std::string v = raw_args_[i + 1];
      for (auto &arg : args_) {
        if (arg.GetShort() == shortName) {
          arg.SetValue(v);
        }
      }
    } else {
      // args_[i].SetValue(raw_args_[i]);
    }
  }

  for (auto const &arg : args_) {
    if (arg.IsRequired() && !arg.GetValue().has_value()) {
      std::cout << "Is required but not set:" << arg.GetName() << ":"
                << std::endl;
    }
  }

  return *this;
}

auto Args::Get(std::string arg_name) const -> std::optional<std::string> {
  for (const auto &argument : args_) {
    if (argument.GetName() == arg_name) {
      return argument.GetValue();
    }
  }

  return std::nullopt;
}

}  // namespace ccap

#endif  // CCAP_CCAP_H