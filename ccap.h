#ifndef CCAP_CCAP_H_
#define CCAP_CCAP_H_

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace ccap {

//
// Defines an argument.
//
class Argument
{
 private:
  bool required_ = false;
  bool expects_value_ = false;
  std::string long_;
  std::string name_;
  char short_;
  std::string value_;

 public:
  // An argument must be initialized with a name
  Argument() = delete;
  explicit Argument(std::string name);
  static auto WithName(std::string name) -> Argument;

  auto GetName() const -> std::string;
  auto GetShort() const -> char;
  auto SetShort(char s) -> Argument &;
  auto GetLong() const -> std::string;
  auto SetLong(std::string l) -> Argument &;
  auto GetValue() const -> std::optional<std::string>;
  auto SetValue(std::string value) -> Argument &;
  auto ExpectsValue() -> Argument &;
  auto IsExpectingValue() const -> bool;
  auto Required() -> Argument &;
  auto IsRequired() const -> bool;
};

Argument::Argument(std::string name) : name_(name){};

auto Argument::WithName(std::string name) -> Argument
{
  return Argument(name);
}

auto Argument::GetName() const -> std::string
{
  return name_;
}

auto Argument::GetShort() const -> char
{
  return short_;
}

auto Argument::SetShort(char s) -> Argument &
{
  short_ = s;
  return *this;
}

auto Argument::GetLong() const -> std::string
{
  return long_;
}

auto Argument::SetLong(std::string l) -> Argument &
{
  long_ = l;
  return *this;
}

auto Argument::SetValue(std::string value) -> Argument &
{
  value_ = value;
  return *this;
}

auto Argument::GetValue() const -> std::optional<std::string>
{
  if (value_.size() > 0) {
    return value_;
  }

  return std::nullopt;
}

auto Argument::ExpectsValue() -> Argument &
{
  expects_value_ = true;
  return *this;
}

auto Argument::IsExpectingValue() const -> bool
{
  return expects_value_;
}

auto Argument::Required() -> Argument &
{
  required_ = true;
  return *this;
}

auto Argument::IsRequired() const -> bool
{
  return required_;
}

enum class TerminationType
{
  Exit,
  Exception
};

// This class takes and stores the raw arguments as well as the
// argument definitions. The class can be initialized in two
// ways (this is more a decision on taste):
//    auto args = Args::from(argc, argv);
//    auto args = Args(argc, argv);
class Args
{
 private:
  // Defines how the program should be terminated
  // when an error ocurres.
  TerminationType terminateBy_ = TerminationType::Exit;

  // The raw arguments from the command line
  std::vector<std::string> raw_args_;

  // The argument definitions
  std::vector<Argument> args_;

  int num_args_;

 public:
  // The count and the arguments are needed in the constructor
  Args() = delete;
  explicit Args(int argc, char const *argv[]);
  static auto From(int argc, char const *argv[]) -> Args;

  // Adds an argument definition
  auto Arg(Argument item) -> Args &;

  // Gets the value of an argument
  auto Get(std::string arg_name) const -> std::optional<std::string>;

  // Parses and validates the given arguments
  // and updates the argument definitions.
  auto Parse() -> Args &;

  // Sets the prefered termination type
  auto SetTerminationType(TerminationType type) -> void;

  auto Terminate(const Argument &arg) -> void;
};

//
// Constructor
// Initialize the class with the raw arguments.
//
Args::Args(int argc, char const *argv[])
{
  for (int i = 1; i < argc; ++i) {
    raw_args_.push_back(argv[i]);
  }

  num_args_ = argc == 0 ? argc : argc - 1;
}

//
// Initialize an Args class with the raw arguments.
//
auto Args::From(int argc, char const *argv[]) -> Args
{
  return Args(argc, argv);
}

//
// Add an argument object.
//
auto Args::Arg(Argument item) -> Args &
{
  args_.push_back(item);
  return *this;
}

//
// Get the value of an argument.
//
auto Args::Get(std::string arg_name) const -> std::optional<std::string>
{
  for (const auto &argument : args_) {
    if (argument.GetName() == arg_name) {
      return argument.GetValue();
    }
  }

  return std::nullopt;
}

//
// Parse the raw arguments and set the according argument objects.
//
auto Args::Parse() -> Args &
{
  for (int i = 0; i < num_args_; ++i) {
    if (raw_args_[i].starts_with("--")) {
      std::string name = raw_args_[i].substr(2);
      std::string v = raw_args_[i + 1];
      for (auto &arg : args_) {
        if (arg.GetLong() == name && arg.IsExpectingValue()) {
          arg.SetValue(v);
        }
      }
    } else if (raw_args_[i].starts_with("-")) {
      char shortName = raw_args_[i][1];
      std::string v = raw_args_[i + 1];
      for (auto &arg : args_) {
        if (arg.GetShort() == shortName && arg.IsExpectingValue()) {
          arg.SetValue(v);
        }
      }
    } else {
      // args_[i].SetValue(raw_args_[i]);
    }
  }

  for (auto const &arg : args_) {
    if (arg.IsRequired() && !arg.GetValue().has_value()) {
      Terminate(arg);
    }
  }

  return *this;
}

auto Args::SetTerminationType(TerminationType type) -> void
{
  terminateBy_ = type;
}

//
// Terminate the program based on the choosen termination type.
//
auto Args::Terminate(const Argument &arg) -> void
{
  switch (terminateBy_) {
    case TerminationType::Exit:
      std::cerr << "Error: Missing required value from argument '"
                << arg.GetName() << "'\n";
      exit(EXIT_FAILURE);

    case TerminationType::Exception: throw "Exception";
  }
}

}  // namespace ccap

#endif  // CCAP_CCAP_H