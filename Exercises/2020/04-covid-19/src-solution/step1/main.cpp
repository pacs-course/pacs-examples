#include "Contagion.hpp"

int
main(int argc, char **argv)
{
  ContagionParameters params;

  Contagion contagion(params);
  contagion.simulate();

  return 0;
}
