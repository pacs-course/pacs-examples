#include "Contagion.hpp"

int
main(int argc, char **argv)
{
  Contagion contagion("params.txt");
  contagion.run();

  return 0;
}
