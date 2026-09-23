// To avoid some warnings in autodiff headers
#pragma GCC diagnostic ignored "-Wdeprecated"
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wparentheses"
#include <Eigen/Core>
#include <Eigen/Dense>

#include <autodiff/forward/dual.hpp>
#include <autodiff/forward/real.hpp>
#include <autodiff/forward/real/eigen.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

// The autodiff namespace contains the forward-mode automatic differentiation
// types and functions. Brings the autodiff namespace into the current scope to
// avoid having to prefix all autodiff types and functions with "autodiff::".
// Bringing namespase names int the global scope is generally discouraged in
// header files, but it is acceptable in source files for convenience.
using namespace autodiff;

namespace
{

// a helper function to print headings for the examples
void
heading(const std::string &title)
{
  std::cout << "\n" << title << "\n" << std::string(title.size(), '=') << "\n";
}

// A scalar function used in the forward examples.
/*! \brief A scalar function used in the forward examples.
    \param x A dual variable.
    \param y A dual variable.
    \return The value of the function (x + y) * exp(x * y).
*/
dual
scalar_forward(dual x, dual y)
{
  return (x + y) * exp(x * y);
}

// A second-order active scalar. Repeating x in wrt(x, x) asks for d^2f/dx^2.
/*! \brief A second-order active scalar.
    \param x A dual2nd variable.
    \return The value of the function sin(x) * exp(x).
*/
dual2nd
scalar_second(dual2nd x)
{
  return sin(x) * exp(x);
}

// Forward-mode Eigen example: a scalar function of an Eigen array.
/*! \brief A scalar function of an Eigen array.
    \param x An Eigen array of real variables.
    \return The value of the function 0.5 * (x * x).sum() + exp(0.2 * x.sum()) +
   x(0) * x(1).
*/
real
energy_forward(const ArrayXreal &x)
{
  return 0.5 * (x * x).sum() + exp(0.2 * x.sum()) + x(0) * x(1);
}

// A nonlinear residual involving an Eigen matrix and vectors.
/*! \brief A nonlinear residual involving an Eigen matrix and vectors.
    \param x An Eigen vector of real variables.
    \param A An Eigen matrix of real variables.
    \param b An Eigen vector of real variables.
    \return The value of the residual.
*/
VectorXreal
residual(const VectorXreal &x, const MatrixXreal &A, const VectorXreal &b)
{
  VectorXreal r = A * x - b;
  for(Eigen::Index i = 0; i < x.size(); ++i)
    r(i) += 0.1 * sin(x(i));
  return r;
}

} // namespace

void
run_forward_examples()
{
  std::cout << std::setprecision(10) << std::fixed;

  heading("Forward-mode examples");

  heading("1. Scalar first derivatives");
  dual x = 1.0;
  dual y = 2.0;
  dual u = scalar_forward(x, y);

  // These two calls correspond conceptually to the forward seeds (1,0)
  // and (0,1), respectively.
  const double ux = derivative(scalar_forward, wrt(x), at(x, y));
  const double uy = derivative(scalar_forward, wrt(y), at(x, y));

  // For x=1 and y=2, the analytical derivatives are 7*exp(2) and 4*exp(2).
  const double ux_exact = 7.0 * std::exp(2.0);
  const double uy_exact = 4.0 * std::exp(2.0);

  std::cout
    << "u             = " << u << '\n'
    << "du/dx (AD)    = " << ux << '\n'
    << "du/dx exact   = " << ux_exact << '\n'
    << "du/dy (AD)    = " << uy << '\n'
    << "du/dy exact   = " << uy_exact << '\n'
    << "What this illustrates: one forward pass can provide the derivative "
       "with respect to one input variable at a time.\n";

  heading("2. Second derivative");
  dual2nd s = 0.5;
  const auto [f0, fx, fxx] = derivatives(scalar_second, wrt(s, s), at(s));

  std::cout
    << "f       = " << f0 << '\n'
    << "df/dx   = " << fx << '\n'
    << "d2f/dx2 = " << fxx << '\n'
    << "What this illustrates: repeated forward seeds can reveal higher-order "
       "derivatives such as second derivatives.\n";

  heading("3. Eigen array gradient");
  ArrayXreal xa(3);
  xa << 1.0, -0.5, 2.0;

  real            energy_value;
  Eigen::VectorXd g_forward;
  gradient(energy_forward, wrt(xa), at(xa), energy_value, g_forward);

  std::cout << "x = " << xa.transpose() << '\n'
            << "E = " << energy_value << '\n'
            << "grad E =\n"
            << g_forward << '\n'
            << "What this illustrates: forward mode can differentiate a scalar "
               "function with respect to a vector of inputs.\n";

  heading("4. Eigen matrix/vector Jacobian");
  MatrixXreal A(3, 3);
  A << 3.0, -1.0, 0.0, -1.0, 3.0, -1.0, 0.0, -1.0, 3.0;

  VectorXreal xv(3);
  VectorXreal b(3);
  xv << 0.2, -0.1, 0.4;
  b << 1.0, 0.0, 1.0;

  VectorXreal     F;
  Eigen::MatrixXd J;
  jacobian(residual, wrt(xv), at(xv, A, b), F, J);

  std::cout << "F(x) =\n"
            << F << '\n'
            << "dF/dx =\n"
            << J << '\n'
            << "What this illustrates: forward mode also handles Jacobians for "
               "vector-valued functions.\n";
}

int
main()
{
  run_forward_examples();
  return 0;
}
