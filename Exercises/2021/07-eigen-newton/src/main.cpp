#include "NewtonTraits.hpp"

int
main(int argc, char **argv)
{
  using ArgumentType       = NewtonTraits::ArgumentType;
  using ReturnType         = NewtonTraits::ReturnType;
  using JacobianMatrixType = NewtonTraits::JacobianMatrixType;

  auto system = [](const ArgumentType &x) -> ReturnType {
    ReturnType y(2);

    y(0) = (x[0] - 1) * (x[0] - 1) + 0.1 * (x[1] - 5) * (x[1] - 5);
    y(1) = 1.5 - x[0] - 0.1 * x[1];

    return y;
  };

  auto jacobianFun = [](const ArgumentType &x) -> JacobianMatrixType {
    JacobianMatrixType J(2, 2);

    J(0, 0) = 2 * (x[0] - 1);
    J(0, 1) = 0.2 * (x[1] - 5);
    J(1, 0) = -1;
    J(1, 1) = -0.1;

    return J;
  };

  return 0;
}
