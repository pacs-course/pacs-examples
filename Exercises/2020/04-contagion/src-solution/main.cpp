#include "Contagion.hpp"

#include <memory>

int
main(int argc, char **argv)
{
  std::shared_ptr<const ContagionParameters> params_contagion =
    std::make_shared<const ContagionParameters>("params.pot");

  Contagion contagion(params_contagion);
  contagion.simulate();

  return 0;
}
