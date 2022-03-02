#ifndef NEWTONTRAITS_HPP
#define NEWTONTRAITS_HPP

#include <Eigen/Dense>

#include <functional>

/// We base the Newton solver on Eigen for simplicity
/// In principle this class should be a template class
/// to enable choosing different type by selecting a different
/// template argument during the instantiation of a Newton template
/// class. For simplicity this is omitted here.
class NewtonTraits
{
public:
  /// The type of argument: an Eigen dynamic vector
  using ArgumentType = Eigen::Matrix<double, Eigen::Dynamic, 1>;

  /// The return type: we want a function \f$R^N\to R^N\f$
  using ReturnType = ArgumentType;

  /// The type for the non-linear system
  using NonLinearSystemType =
    std::function<ReturnType(const ArgumentType &)>;

  /// Type used to store the Jacobian as matrix
  using JacobianMatrixType = Eigen::MatrixXd;

  /// Type for the JacobianFunction
  using JacobianFunctionType =
    std::function<JacobianMatrixType(const ArgumentType &)>;
};

#endif /* NEWTONTRAITS_HPP */
