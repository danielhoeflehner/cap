#ifndef GRRS_ARGUMENT_H_
#define GRRS_ARGUMENT_H_

#include <string>

class Argument
{
private:
  std::string long_;
  std::string name_;
  char short_;
  std::string value_;

public:
  Argument() = delete;
  Argument(std::string name);
  static Argument WithName(std::string name);
  auto SetShort(char s) -> Argument &;
  auto SetLong(std::string l) -> Argument &;
  auto SetValue(std::string value) -> Argument &;
  auto GetValue() const -> std::string;
  auto GetName() const -> std::string;
};

#endif // GRRS_ARGUMENT_H_
