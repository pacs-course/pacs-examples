#include "GetPot"

#include <iostream>
#include <string>

int
main(int argc, char **argv)
{
  GetPot datafile("data");

  const std::string global_section = "integration/";
  const std::string section1       = global_section + "domain/";
  const std::string section2       = global_section + "mesh/";

  double a        = datafile((section1 + "a").data(), 0.);
  double b        = datafile((section1 + "b").data(), 1.);
  int n_intervals = datafile((section2 + "n_intervals").data(), 10);

  std::cout << a << " " << b << " " << n_intervals << std::endl;
  return 0;
}
