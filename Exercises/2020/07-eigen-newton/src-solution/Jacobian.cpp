#include "Jacobian.hpp"

DiscreteJacobian::ArgumentType
DiscreteJacobian::solve(const ArgumentType &x,
                        const ArgumentType &b) const
{
  std::size_t M = x.size();

  DiscreteJacobian::JacobianMatrixType J(M, M);

  for (std::size_t i = 0; i < M; ++i)
    {
      // Fill i-th column with
      // the partial derivative of f with respect to x_i,
      // evaluated with central finite differences of step M_tol.
      ArgumentType x1(x);
      ArgumentType x2(x);

      x1[i] -= M_tol;
      x2[i] += M_tol;

      ArgumentType f1 = (*M_sys)(x1);
      ArgumentType f2 = (*M_sys)(x2);

      J.col(i) = (f2 - f1) / (2 * M_tol);
    }

  return J.fullPivLu().solve(b);
}

FullJacobian::ArgumentType
FullJacobian::solve(const ArgumentType &x,
                    const ArgumentType &b) const
{
  FullJacobian::JacobianMatrixType J = this->Jac(x);
  return J.fullPivLu().solve(b);
}
