#include "GetPot"

#include <iostream>

int main(int argc, char **argv) {
  GetPot command_line(argc, argv);

  const double a           = command_line("a", 0.);
  const double b           = command_line("b", 1.);
  const int    n_intervals = command_line("n_intervals", 10);

  std::cout << a << " " << b << " " << n_intervals << std::endl;

  return 0;
}
