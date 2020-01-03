#include "argument.h"

#include <string>

Argument::Argument(std::string name) : name_(name){};

Argument Argument::WithName(std::string name)
{
  return Argument(name);
}

auto Argument::SetShort(char s) -> Argument &
{
  short_ = s;
  return *this;
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

auto Argument::GetValue() const -> std::string
{
  return value_;
}

auto Argument::GetName() const -> std::string
{
  return name_;
}
