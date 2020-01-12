#include "cap.h"

#include <iostream>

using namespace cap;

int main(int argc, char const *argv[]) {
  Args args = Args::From(argc, argv)
                  .Arg(Argument::WithName("file").SetShort('f').SetLong("file"))
                  .Arg(Argument::WithName("path").SetShort('p').SetLong("path"))
                  .Parse();

  if (auto fileValue = args.Get("file")) {
    std::cout << fileValue.value() << std::endl;
  }

  if (auto pathValue = args.Get("path")) {
    std::cout << pathValue.value() << std::endl;
  }

  return 0;
}
