// to avoid some warnings in autodiff headers
#pragma GCC diagnostic ignored "-Wdeprecated"
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wparentheses"
#include <Eigen/Core>
#include <Eigen/Dense>

#include <autodiff/reverse/var.hpp>
#include <autodiff/reverse/var/eigen.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

// The autodiff namespace contains the reverse-mode automatic differentiation
// types and functions. Brings the autodiff namespace into the current scope to
// avoid having to prefix all autodiff types and functions with "autodiff::".
// Bringing namespase names int the global scope is generally discouraged in
// header files, but it is acceptable in source files for convenience.
using namespace autodiff;

namespace
{
/** \brief Print a section heading. */
void
heading(const std::string &title)
{
  std::cout << "\n" << title << "\n" << std::string(title.size(), '=') << "\n";
}
/** \brief A scalar function for reverse-mode automatic differentiation.
 *   \param x A var variable.
    \param y A var variable.
    \return The value of the function (x + y) * exp(x * y).
*/
var
scalar_reverse(var x, var y)
{
  return (x + y) * exp(x * y);
}
/** \brief A scalar function for reverse-mode automatic differentiation.
 *   \param x An Eigen vector of var variables.
 *   \return The value of the function 0.5 * (x * x).sum() + exp(0.2 * x.sum())
 * + x(0) * x(1).
 */
var
energy_reverse(const VectorXvar &x)
{
  var sum = 0.0;
  var sumsq = 0.0;

  for(Eigen::Index i = 0; i < x.size(); ++i)
    {
      sum += x(i);
      sumsq += x(i) * x(i);
    }

  return 0.5 * sumsq + exp(0.2 * sum) + x(0) * x(1);
}

} // namespace

int
main()
{
  std::cout << std::setprecision(10) << std::fixed;

  heading("Reverse-mode examples");

  heading("1. Scalar partial derivatives");
  var xr = 1.0;
  var yr = 2.0;
  var ur = scalar_reverse(xr, yr);

  const auto [urx, ury] = derivatives(ur, wrt(xr, yr));

  std::cout
    << "u       = " << ur << '\n'
    << "du/dx   = " << urx << '\n'
    << "du/dy   = " << ury << '\n'
    << "What this illustrates: reverse mode accumulates gradients for all "
       "input variables in one sweep.\n";

  heading("2. Eigen vector gradient and Hessian");
  VectorXvar xh(3);
  xh << 1.0, -0.5, 2.0;

  var                   eh = energy_reverse(xh);
  Eigen::VectorXd       g_reverse;
  const Eigen::MatrixXd H = hessian(eh, xh, g_reverse);

  std::cout
    << "E = " << eh << '\n'
    << "grad E =\n"
    << g_reverse << '\n'
    << "Hessian E =\n"
    << H << '\n'
    << "What this illustrates: reverse mode is especially efficient for "
       "gradient and Hessian computations in higher dimensions.\n";

  return 0;
}
