#ifndef NEWTONMETHODSSUPPORT_HPP
#define NEWTONMETHODSSUPPORT_HPP

#include "NewtonTraits.hpp"
#include <Eigen/Dense>

/// The input options, with default values.
///
/// \note Absolute tolerances are used in the code.
/// It is user's task to set them properly.
class NewtonOptions
{
public:
  /// Tolerance for two successive iterations.
  /// The iteration stops if
  /// \f$||x_{new}-x_{old}|| < tolerance\f$.
  double tolerance{1.e-8};

  /// Tolerance on residual.
  /// The iteration stops if \f$||F(x)|| < minRes\f$.
  double minRes{1.e-6};

  /// Max. number of iterations.
  unsigned int maxIter{50};

  /// Stop if stagnation occurs, i.e. if for two consecutive
  /// iterations the residual does not decrease.
  bool stopOnStagnation{false};
};

/// The output results.
class NewtonResult
{
public:
  /// The solution.
  NewtonTraits::ReturnType solution;

  /// Last norm of residual.
  double residualNorm{0.0};

  /// The last step length.
  double stepLength{0.0};

  /// The last value of the iteration counter.
  unsigned int iterations{0u};

  /// Convergence flag. True if convergence is reached, i.e.
  /// if both conditions on residual and step length are met.
  bool converged = false;

  /// Stagnation flag. True if stagnation occurred.
  bool stagnation{false};
};

#endif /* NEWTONMETHODSSUPPORT_HPP */
