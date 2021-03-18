#include "GetPot"

#include <iostream>

int
main(int argc, char **argv)
{
  GetPot command_line(argc, argv);

  double a           = command_line("a", 0.);
  double b           = command_line("b", 1.);
  int    n_intervals = command_line("n_intervals", 10);

  std::cout << a << " " << b << " " << n_intervals << std::endl;
  return 0;
}
