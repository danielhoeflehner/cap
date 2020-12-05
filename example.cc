#include <iostream>

#include "ccap.h"

using namespace ccap;

int main(int argc, char const *argv[]) {
  Args args =
      Args::From(argc, argv)
          .Arg(
              Flag::WithName("file")
                  .SetShort('f')    // Short is optional and defaults to the
                                    // first letter of the name
                  .SetLong("file")  // Long is optional and defaults to the name
                  .ExpectsValue()
                  .Required())
          .Arg(Flag::WithName("path").ExpectsValue())
          .Arg(Flag::WithName("verbose").SetShort('v').SetLong("verbose"))
          .Parse();

  // We can be sure to get a value, because we defined
  // the "file" argument to be required.
  std::cout << "File flag is: " << args.Get("file").value() << std::endl;

  if (auto pathValue = args.Get("path")) {
    std::cout << "Path flag is: " << pathValue.value() << std::endl;
  }

  if (args.IsGiven("verbose")) {
    std::cout << "Verbose flag is set...\n";
  }

  return 0;
}
