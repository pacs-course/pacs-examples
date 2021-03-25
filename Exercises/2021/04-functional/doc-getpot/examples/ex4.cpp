#include "GetPot"

#include <iostream>
#include <string>

int
main(int argc, char **argv)
{
  GetPot            command_line(argc, argv);
  const std::string filename = command_line.follow("data", 2, "-f", "--file");

  GetPot            datafile(filename.c_str());
  const std::string section = "integration/domain/";

  double a = datafile((section + "a").data(), 0.);
  /* ... */
  return 0;
}
