#include "GetPot"

#include <iostream>

int main(int argc, char **argv) {
  GetPot datafile("data");

  const double a           = datafile("integration/domain/a", 0.);
  const double b           = datafile("integration/domain/b", 1.);
  const int    n_intervals = datafile("integration/mesh/n_intervals", 10);

  std::cout << a << " " << b << " " << n_intervals << std::endl;

  return 0;
}
