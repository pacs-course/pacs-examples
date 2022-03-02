#ifndef NEWTONTRAITS_HPP
#define NEWTONTRAITS_HPP

#include <functional>

#include <Eigen/Dense>

/// Enumerator for scalar or vector problem types.
///
/// A simple enumerator is easily convertible to an int.
/// The numbering is explicitely indicated, only to facilitate the
/// identification of the integer corresponding to the enum (if it should be
/// parsed from a file, for instance).
enum class ProblemType : unsigned int
{
  Scalar = 0,
  Vector = 1
};

template <ProblemType Type>
class NewtonTraits
{};

/// Specialization for scalar problems.
template <>
class NewtonTraits<ProblemType::Scalar>
{
public:
  /// Type of variable: a double.
  using VariableType = double;

  /// Type used to store the Jacobian.
  using JacobianMatrixType = double;

  /// The type for the non-linear system.
  using NonLinearSystemType = std::function<VariableType(const VariableType &)>;

  /// Type for evaluating the Jacobian.
  using JacobianFunctionType =
    std::function<JacobianMatrixType(const VariableType &)>;

  /// Static method to compute the size of a variable.
  static size_t
  size(const VariableType & /*x*/)
  {
    return 1;
  }

  /// Static method to compute the norm of a variable.
  static double
  norm(const VariableType &x)
  {
    return std::abs(x);
  }
};

/// Specialization for vector problems.
template <>
class NewtonTraits<ProblemType::Vector>
{
public:
  /// Type of variable: an Eigen dynamic vector.
  using VariableType = Eigen::VectorXd;

  /// Type used to store the Jacobian.
  using JacobianMatrixType = Eigen::MatrixXd;

  /// The type for the non-linear system.
  using NonLinearSystemType = std::function<VariableType(const VariableType &)>;

  /// Type for evaluating the Jacobian.
  using JacobianFunctionType =
    std::function<JacobianMatrixType(const VariableType &)>;

  /// Static method to compute the size of a variable.
  static size_t
  size(const VariableType &x)
  {
    return x.size();
  }

  /// Static method to compute the norm of a variable.
  static double
  norm(const VariableType &x)
  {
    return x.norm();
  }
};

#endif /* NEWTONTRAITS_HPP */
