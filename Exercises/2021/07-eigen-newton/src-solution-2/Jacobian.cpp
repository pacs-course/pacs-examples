#include "Jacobian.hpp"

/// Specialization for scalar problems.
template <>
typename NewtonTraits<ProblemType::Scalar>::VariableType
DiscreteJacobian<ProblemType::Scalar>::solve(
  const typename T::VariableType &x,
  const typename T::VariableType &b) const
{
  typename T::VariableType x_m(x);
  typename T::VariableType x_p(x);

  x_m -= h;
  x_p += h;

  const typename T::VariableType f_m = system(x_m);
  const typename T::VariableType f_p = system(x_p);

  const double df_dx = (f_p - f_m) / (2 * h);

  return b / df_dx;
}

/// Specialization for vector problems.
template <>
typename NewtonTraits<ProblemType::Vector>::VariableType
DiscreteJacobian<ProblemType::Vector>::solve(
  const typename T::VariableType &x,
  const typename T::VariableType &res) const
{
  const size_t n = x.size();

  typename T::JacobianMatrixType J(n, n);

  for (size_t i = 0; i < n; ++i)
    {
      typename T::VariableType x_m(x);
      typename T::VariableType x_p(x);

      // Fill i-th column with
      // the partial derivative of f with respect to x_i,
      // evaluated with central finite differences of step h.
      x_m[i] -= h;
      x_p[i] += h;

      const typename T::VariableType f_m = system(x_m);
      const typename T::VariableType f_p = system(x_p);

      J.col(i) = (f_p - f_m) / (2 * h);
    }

  return J.fullPivLu().solve(res);
}

/// Specialization for scalar problems.
template <>
typename NewtonTraits<ProblemType::Scalar>::VariableType
FullJacobian<ProblemType::Scalar>::solve(
  const typename T::VariableType &x,
  const typename T::VariableType &b) const
{
  const typename T::JacobianMatrixType J = jac(x);

  return b / J;
}

/// Specialization for vector problems.
template <>
typename NewtonTraits<ProblemType::Vector>::VariableType
FullJacobian<ProblemType::Vector>::solve(
  const typename T::VariableType &x,
  const typename T::VariableType &res) const
{
  const typename T::JacobianMatrixType J = jac(x);

  return J.fullPivLu().solve(res);
}
