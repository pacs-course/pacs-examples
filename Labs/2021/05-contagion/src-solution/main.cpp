#include "Contagion.hpp"

int
main(int argc, char **argv)
{
  Contagion contagion("params.pot");
  contagion.run();

  return 0;
}
