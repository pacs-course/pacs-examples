int
main(int argc, char **argv)
{
  using ArgumentType = NewtonTraits::ArgumentType;
  using JacobianType = NewtonTraits::JacobianMatrixType;

  auto system = [](const ArgumentType &x) -> ArgumentType {
    ArgumentType y(2);

    y(0) = (1 - x(0)) * (1 - x(0)) + 0.1 * (5 - x(1)) * (5 - x(1));
    y(1) = 1.5 - x(0) - 0.1 * x(1);

    return y;
  };

  auto jacobianFun = [](const ArgumentType &x) -> JacobianType {
    JacobianType J(2, 2);

    J(0, 0) = -2 * (1 - x(0));
    J(0, 1) = -0.2 * (5 - x(1));
    J(1, 0) = -1;
    J(1, 1) = -0.1;

    return J;
  };

  return 0;
}
