#ifndef RKFTRAITS_HPP
#define RKFTRAITS_HPP

#include "Eigen/Dense"

#include <cmath>
#include <functional>

/// The three kinds of RK solver for scalar and vector differential problems.
class RKFType
{
public:
  using Scalar = double;
  using Vector = Eigen::VectorXd;
};

/// Primary class template.
/// This class is in principle also valid for matrix problems.
template <class ProblemType>
class RKFTraits
{
public:
  using VariableType = ProblemType;

  using ForcingTermType =
    std::function<VariableType(const double &, const VariableType &)>;

  static double
  norm(const VariableType &x)
  {
    return x.norm();
  }
};

/// Specialization for scalar problems.
template <>
struct RKFTraits<double>
{
  using VariableType = double;

  using ForcingTermType =
    std::function<VariableType(const double &, const VariableType &)>;

  static double
  norm(const VariableType &x)
  {
    return std::abs(x);
  }
};

#endif /* RKFTRAITS_HPP */
