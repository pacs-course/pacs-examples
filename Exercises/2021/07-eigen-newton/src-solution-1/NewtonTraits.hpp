#ifndef NEWTONTRAITS_HPP
#define NEWTONTRAITS_HPP

#include <functional>

#include <Eigen/Dense>

/// Class defining common types for homogeneity.
class NewtonTraits
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
};

#endif /* NEWTONTRAITS_HPP */
