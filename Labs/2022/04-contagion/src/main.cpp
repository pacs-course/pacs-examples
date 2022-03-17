#include "Contagion.hpp"

int
main(int argc, char **argv)
{
  Contagion contagion("params.dat");
  contagion.run();

  return 0;
}
