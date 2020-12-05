#ifndef CCAP_CCAP_H_
#define CCAP_CCAP_H_

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace ccap {

//
// -- Definitions
//

enum class TerminationType { Exit, Exception };

class Flag {
 public:
  Flag() = delete;
  explicit Flag(const std::string &name);
  static auto WithName(const std::string &name) -> Flag;

  auto GetName() const -> const std::string &;
  auto GetShort() const -> char;
  auto SetShort(char s) -> Flag &;
  auto GetLong() const -> const std::string &;
  auto SetLong(const std::string &l) -> Flag &;
  auto GetValue() const -> std::optional<std::string>;
  auto SetValue(std::optional<std::string> value) -> Flag &;
  auto ExpectsValue() -> Flag &;
  auto IsExpectingValue() const -> bool;
  auto Required() -> Flag &;
  auto IsRequired() const -> bool;

  auto IsOption() const -> bool;
  auto IsGiven() const -> bool;
  auto SetGiven(bool value);

 private:
  std::string name_;
  std::string long_;
  char short_ = 0;
  std::optional<std::string> value_ = std::nullopt;

  bool required_ = false;
  bool expects_value_ = false;
  bool is_option_ = true;
  bool is_given_ = false;
};

class Args {
 public:
  Args() = delete;
  explicit Args(int argc, char const *argv[]);
  ~Args(){};

  static auto From(int argc, char const *argv[]) -> Args;

  // Adds an Flag
  auto Arg(Flag item) -> Args &;

  // Gets the value of an Flag
  auto Get(const std::string &arg_name) const -> std::optional<std::string>;

  auto IsGiven(const std::string &arg_name) const -> bool;

  // Parses and validates the given Flags
  // and updates the Flag definitions.
  auto Parse() -> Args &;

  // Sets the prefered termination type
  auto SetTerminationType(TerminationType type) -> void;

  auto SetAbout(const std::string &version) -> Args &;
  auto SetAuthor(const std::string &author) -> Args &;
  auto SetName(const std::string &name) -> Args &;
  auto SetVersion(const std::string &version) -> Args &;

  auto ShowHelp() -> void;

  auto Terminate(const Flag &arg) -> void;

 private:
  std::vector<std::string> raw_args_;
  std::vector<Flag> args_;
  TerminationType terminateBy_ = TerminationType::Exit;
  int num_args_;
  std::string about_;
  std::string author_;
  std::string name_;
  std::string version_ = "0.0.1";

  auto GetValueFromRawArgs(int raw_arg_index) -> std::optional<std::string>;
  auto ReadLongArg(int raw_arg_index) -> void;
  auto ReadShortArg(int raw_arg_index) -> void;
};

//
// -- Declarations
//

Flag::Flag(const std::string &name)
    : name_{name}, short_{name.at(0)}, long_{name} {};

auto Flag::WithName(const std::string &name) -> Flag { return Flag(name); }

auto Flag::GetName() const -> const std::string & { return name_; }

auto Flag::GetShort() const -> char { return short_; }

auto Flag::SetShort(char s) -> Flag & {
  short_ = s;
  return *this;
}

auto Flag::GetLong() const -> const std::string & { return long_; }

auto Flag::SetLong(const std::string &l) -> Flag & {
  long_ = l;
  return *this;
}

auto Flag::SetValue(std::optional<std::string> value) -> Flag & {
  value_ = value;
  return *this;
}

auto Flag::GetValue() const -> std::optional<std::string> { return value_; }

auto Flag::ExpectsValue() -> Flag & {
  expects_value_ = true;

  // If the Flags expects a value,
  // it cannot be a boolean flag.
  is_option_ = false;

  return *this;
}

auto Flag::IsExpectingValue() const -> bool { return expects_value_; }

auto Flag::Required() -> Flag & {
  required_ = true;
  return *this;
}

auto Flag::IsRequired() const -> bool { return required_; }

auto Flag::IsOption() const -> bool { return is_option_; }

auto Flag::IsGiven() const -> bool { return is_given_; }

auto Flag::SetGiven(bool value) { is_given_ = value; }

// Initialize the class with the raw Flags.
Args::Args(int argc, char const *argv[]) {
  // Start at 1 because we don't need the program name
  for (int i = 1; i < argc; ++i) {
    raw_args_.push_back(argv[i]);
  }

  num_args_ = argc == 0 ? argc : argc - 1;
}

// Create an Args instance with the raw Flags.
auto Args::From(int argc, char const *argv[]) -> Args {
  return Args(argc, argv);
}

// Add an Flag instance.
auto Args::Arg(Flag item) -> Args & {
  args_.push_back(item);
  return *this;
}

// Get the value of an Flag.
auto Args::Get(const std::string &arg_name) const
    -> std::optional<std::string> {
  for (const auto &Flag : args_) {
    if (Flag.GetName() == arg_name) {
      return Flag.GetValue();
    }
  }

  return std::nullopt;
}

// Check if the option arg is given.
auto Args::IsGiven(const std::string &arg_name) const -> bool {
  for (const auto &Flag : args_) {
    if (Flag.GetName() == arg_name) {
      return Flag.IsOption() && Flag.IsGiven();
    }
  }

  return false;
}

// Parse the raw Flags and set the according Flag objects.
auto Args::Parse() -> Args & {
  for (int i = 0; i < num_args_; ++i) {
    if (raw_args_[i].starts_with("--")) {
      ReadLongArg(i);
    } else if (raw_args_[i].starts_with("-")) {
      ReadShortArg(i);
    } else {
      // args_[i].SetValue(raw_args_[i]);
    }
  }

  // After parsing the raw Flags we need to check that
  // all args that are marked as 'required' do have values.
  for (auto const &arg : args_) {
    if (arg.IsRequired() && !arg.GetValue().has_value()) {
      Terminate(arg);
    }
  }

  return *this;
}

auto Args::GetValueFromRawArgs(int index) -> std::optional<std::string> {
  if (index >= raw_args_.size()) {
    return std::nullopt;
  }

  return raw_args_[index];
}

auto Args::ReadLongArg(int raw_arg_index) -> void {
  // To get the name we "cut" away the "--"
  // e.g.: --name => name
  std::string name = raw_args_[raw_arg_index].substr(2);
  if (name.length() == 0) {
    return;
  }

  if (name == "help") {
    ShowHelp();
  }

  // Find the arg with the given name
  for (auto &arg : args_) {
    if (arg.GetLong() == name) {
      // If the flag expects an value we take the value after
      // the falg name as that value.
      if (arg.IsExpectingValue()) {
        arg.SetValue(GetValueFromRawArgs(raw_arg_index + 1));
      }

      // If the flag is an option we
      // mark the flag to be given.
      if (arg.IsOption()) {
        arg.SetGiven(true);
      }
    }
  }
}

auto Args::ReadShortArg(int raw_arg_index) -> void {
  char shortName = raw_args_[raw_arg_index][1];
  // Retrun when after the '-' nothing is provided
  if (shortName == 0) {
    return;
  }

  if (shortName == 'h') {
    ShowHelp();
  }

  for (auto &arg : args_) {
    if (arg.GetShort() == shortName) {
      if (arg.IsExpectingValue()) {
        arg.SetValue(GetValueFromRawArgs(raw_arg_index + 1));
      }

      if (arg.IsOption()) {
        arg.SetGiven(true);
      }
    }
  }
}

auto Args::SetTerminationType(TerminationType type) -> void {
  terminateBy_ = type;
}

auto Args::SetAbout(const std::string &about) -> Args & {
  about_ = about;
  return *this;
}

auto Args::SetAuthor(const std::string &author) -> Args & {
  author_ = author;
  return *this;
}

auto Args::SetName(const std::string &name) -> Args & {
  name_ = name;
  return *this;
}

auto Args::SetVersion(const std::string &version) -> Args & {
  version_ = version;
  return *this;
}

auto Args::ShowHelp() -> void {
  std::cout << "TODO: Help Message\n";
  exit(0);
}

// Terminate the program based on the choosen termination type.
auto Args::Terminate(const Flag &arg) -> void {
  switch (terminateBy_) {
    case TerminationType::Exit:
      std::cerr << "Error: Missing required value for flag '" << arg.GetName()
                << "'\n";
      exit(EXIT_FAILURE);

    case TerminationType::Exception:
      throw "Exception";
  }
}

}  // namespace ccap

#endif  // CCAP_CCAP_H