#include "ccap.h"

#include <iostream>

using namespace ccap;

int main(int argc, char const *argv[]) {
  Args args = Args::From(argc, argv)
                  .Arg(Argument::WithName("file")
                           .SetShort('f')
                           .SetLong("file")
                           .ExpectsValue()
                           .Required())
                  .Arg(Argument::WithName("path")
                           .SetShort('p')
                           .SetLong("path")
                           .ExpectsValue()
                           .Required())
                  .Arg(Argument::WithName("this")
                           .SetShort('t')
                           .SetLong("this")
                           .Required())
                  .Parse();

  if (auto fileValue = args.Get("file")) {
    std::cout << fileValue.value() << std::endl;
  }

  if (auto pathValue = args.Get("path")) {
    std::cout << pathValue.value() << std::endl;
  }

  if (args.Get("this")) {
    std::cout << "This is set...\n";
  }

  return 0;
}
