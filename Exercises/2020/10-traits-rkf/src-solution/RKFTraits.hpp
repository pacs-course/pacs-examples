#ifndef RKFTRAITS_HPP
#define RKFTRAITS_HPP

#include "Eigen/Dense"

#include <cmath>
#include <functional>
#include <vector>

//! The three kinds of RK solver for scalar, vector and matrix
//! differential problems
enum class RKFKind
{
  SCALAR = 0,
  VECTOR = 1,
  MATRIX = 2
};

//! Primary template
template <RKFKind Kind>
struct RKFTraits
{};

//! Specialization for scalar
template <>
struct RKFTraits<RKFKind::SCALAR>
{
  using VariableType = double;
  using ForcingTermType =
    std::function<VariableType(double const &, VariableType const &)>;
  static double
  norm(VariableType const &x)
  {
    return std::abs(x);
  }
};

//! Specialization for vectors
template <>
struct RKFTraits<RKFKind::VECTOR>
{
  using VariableType = Eigen::VectorXd;
  using ForcingTermType =
    std::function<VariableType(double const &, VariableType const &)>;
  static double
  norm(VariableType const &x)
  {
    return x.norm();
  }
};

//! Specialization for matrices
template <>
struct RKFTraits<RKFKind::MATRIX>
{
  using VariableType = Eigen::MatrixXd;
  using ForcingTermType =
    std::function<VariableType(double const &, VariableType const &)>;
  static double
  norm(VariableType const &x)
  {
    return x.norm();
  }
};

#endif /* RKFTRAITS_HPP */
