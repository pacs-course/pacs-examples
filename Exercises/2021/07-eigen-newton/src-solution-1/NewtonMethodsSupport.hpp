#ifndef NEWTONMETHODSSUPPORT_HPP
#define NEWTONMETHODSSUPPORT_HPP

/// Newton solver options, with default values.
///
/// @note Absolute tolerances are used in the code.
/// It is user's task to set them properly.
class NewtonOptions
{
public:
  /// Tolerance on residual.
  /// The iteration stops if @f$||F(x)|| < tol_res@f$.
  double tol_res = 1e-6;

  /// Tolerance on increment.
  /// The method stops if
  /// @f$||x_{new}-x_{old}|| < tol_incr@f$.
  double tol_incr = 1e-8;

  /// Max. number of iterations.
  unsigned int max_iter = 50;

  /// Stop if stagnation occurs, i.e. if for two consecutive
  /// iterations the residual does not decrease.
  bool stop_on_stagnation = false;
};

/// Output results.
class NewtonResult
{
public:
  /// Solution.
  NewtonTraits::VariableType solution;

  /// Residual norm.
  double norm_res = 0.0;

  /// Increment norm.
  double norm_incr = 0.0;

  /// Iteration counter.
  unsigned int iteration = 0;

  /// Convergence flag. True if convergence is reached, i.e.
  /// if both conditions on residual and increment are met.
  bool converged = false;

  /// Stagnation flag. True if stagnation occurred.
  bool stagnation = false;
};

#endif /* NEWTONMETHODSSUPPORT_HPP */
