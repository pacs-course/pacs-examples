#include "Newton.hpp"

#include <exception>
#include <iostream>
#include <limits>

NewtonResult
Newton::solve(const T::VariableType &x0)
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
  if (solution.size() != residual.size())
    throw std::runtime_error("Newton needs a function from Rn to Rn");

  norm_res = residual.norm();

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

      norm_incr = delta.norm();
      solution -= delta;

      residual = this->system(solution);

      norm_res = residual.norm();

      // If residual does not decrease for two consecutive iterations
      // mark for stagnation.
      const bool no_decrease = (norm_res >= norm_res_old);
      stagnation             = (no_decrease_old && no_decrease);
      no_decrease_old        = no_decrease;

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
