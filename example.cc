#include <iostream>

#include "ccap.h"

using namespace ccap;

int main(int argc, char const *argv[])
{
  Args args =
      Args::From(argc, argv)
          .Arg(Argument::WithName("file")
                   .SetShort('f')
                   .SetLong("file")
                   .ExpectsValue()
                   .Required())
          .Arg(Argument::WithName("path")
                   .SetShort('p')
                   .SetLong("path")
                   .ExpectsValue())
          .Arg(Argument::WithName("verbose").SetShort('v').SetLong("verbose"))
          .Parse();

  // We can be sure to get a value, because we defined
  // the "file" argument to be required.
  std::cout << "File argument is: " << args.Get("file").value() << std::endl;

  if (auto pathValue = args.Get("path")) {
    std::cout << "Path argument is: " << pathValue.value() << std::endl;
  }

  if (args.IsGiven("verbose")) {
    std::cout << "Verbose flag is set...\n";
  }

  return 0;
}
