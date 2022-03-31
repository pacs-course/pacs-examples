#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "Jacobian.hpp"
#include "NewtonMethodsSupport.hpp"

#include <exception>
#include <iostream>
#include <limits>
#include <memory>

/// This class implements Newton's method.
///
/// The non-linear system to be solved is composed to this class as
/// a private member, whereas the Jacobian is stored as a unique_ptr
/// to the base class. However, as a result the class is neither
/// copy-constructible nor copy-assignable. To have copy operators that perform
/// deep copy a clone() method should be implemented in the Jacobian classes.
template <ProblemType Type>
class Newton
{
public:
  /// Short-hand alias.
  using T = NewtonTraits<Type>;

  /// Default constructor.
  Newton() = default;

  /// This class is not meant to be copy constructable.
  /// The implementation of copy constructor would require a
  /// mechanism for the deep copy of the Jacobian, which is stored
  /// as a unique pointer.
  Newton(const Newton &) = delete;

  /// This constructor accepts a non linear system and a unique
  /// pointer to the Jacobian base class.
  Newton(typename T::NonLinearSystemType &&  system_,
         std::unique_ptr<JacobianBase<Type>> jac_,
         const NewtonOptions &               options_ = NewtonOptions())
    : system(std::forward<typename T::NonLinearSystemType>(system_))
    , jac(std::move(jac_))
    , options(options_)
  {}

  template <class Jac>
  Newton(typename T::NonLinearSystemType &&system_,
         Jac                               jac_,
         const NewtonOptions &             options_ = NewtonOptions())
    : system(std::forward<typename T::NonLinearSystemType>(system_))
    , jac(std::make_unique<JacobianBase<Type>>(jac_))
    , options(options_)
  {}

  virtual ~Newton() = default;

  /// You can set options.
  void
  set_options(const NewtonOptions &options_)
  {
    options = options_;
  }

  NewtonResult<Type>
  solve(const typename T::VariableType &x0);

private:
  typename T::NonLinearSystemType     system;
  std::unique_ptr<JacobianBase<Type>> jac;
  NewtonOptions                       options;
  NewtonResult<Type>                  result;
};


template <ProblemType Type>
NewtonResult<Type>
Newton<Type>::solve(const typename T::VariableType &x0)
{
  // C++17 structured binding.
  // Get all options and result.
  const auto &[tol_res, tol_incr, max_iter, stop_on_stagnation] = this->options;

  auto &[solution, norm_res, norm_incr, iteration, converged, stagnation] =
    this->result;

  // The initial step is to compute the relevant quantities
  // from the initial conditions.
  solution  = x0;
  iteration = 0;
  norm_res  = std::numeric_limits<double>::max();
  norm_incr = std::numeric_limits<double>::max();

  auto residual = this->system(solution);

  // Test if we have a map Rn -> Rn.
  if (T::size(solution) != T::size(residual))
    throw std::runtime_error("Newton needs a function from Rn to Rn");

  norm_res = T::norm(residual);

  // Initial values for tests of convergence and stagnation.
  converged  = false;
  stagnation = false;

  bool stop            = true;
  bool no_decrease_old = true;

  for (iteration = 0; iteration < max_iter; ++iteration)
    {
      auto norm_res_old = norm_res;

      // Compute the increment.
      auto delta = jac->solve(solution, residual);

      norm_incr = T::norm(delta);
      solution -= delta;

      residual = this->system(solution);

      norm_res = T::norm(residual);

      // If residual does not decrease for two consecutive iterations
      // mark for stagnation.
      const bool no_decrease = (norm_res >= norm_res_old);
      stagnation       = (no_decrease_old && no_decrease);
      no_decrease_old  = no_decrease;

      // We stop on stagnation if we decided to.
      stop = (stagnation && stop_on_stagnation);

      // Test convergence.
      converged = ((norm_res <= tol_res) && (norm_incr <= tol_incr));

      std::cout << "    Iteration " << iteration;
      std::cout << ", residual: " << norm_res;
      std::cout << ", increment: " << norm_incr;
      std::cout << std::endl;

      if (converged || stop)
        break;
    }

  return result;
}

#endif /* NEWTON_HPP_ */
