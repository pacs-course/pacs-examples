/*
 * test_polyzero.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: forma
 */
#include "polyHolder.hpp"
#include <cmath>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
int main()
{
  using namespace apsc;
  using Coefficients = PolyHolder::Coefficients;

  auto formatComplex = [](std::complex<double> const &z, int precision = 6,
                          bool scientific = false, bool showPos = false) {
    if(scientific)
      {
        if(precision == 2)
          {
            return showPos ? std::format("({:+.2e},{:+.2e})", z.real(), z.imag())
                           : std::format("({:.2e},{:.2e})", z.real(), z.imag());
          }
        return showPos ? std::format("({:+.6e},{:+.6e})", z.real(), z.imag())
                       : std::format("({:.6e},{:.6e})", z.real(), z.imag());
      }
    if(precision == 2)
      {
        return showPos ? std::format("({:+.2f},{:+.2f})", z.real(), z.imag())
                       : std::format("({:.2f},{:.2f})", z.real(), z.imag());
      }
    return showPos ? std::format("({:+.6f},{:+.6f})", z.real(), z.imag())
                   : std::format("({:.6f},{:.6f})", z.real(), z.imag());
  };

  auto polynomialToString = [&](Coefficients const &coeff) {
    std::ostringstream stream;
    for(std::size_t exp = 0; exp < coeff.size(); ++exp)
      {
        stream << formatComplex(coeff[exp]) << " x^" << exp;
        if(exp + 1u < coeff.size())
          {
            stream << " + ";
          }
      }
    return stream.str();
  };

  auto printRootTable = [&](std::string_view title, Coefficients const &coeff,
                            std::vector<std::complex<double>> const &zeros,
                            std::vector<double> const &residuals) {
    std::cout << std::format("\n{}\n", title);
    std::cout << std::format("{:<28} {:>14} {:>14}\n", "Zero", "Residual",
                             "|p(zero)|");
    std::cout << std::format("{:-<28} {:-<14} {:-<14}\n", "", "", "");
    for(std::size_t k = 0; k < zeros.size(); ++k)
      {
        auto const trueResidual = std::abs(polyEval(coeff, zeros[k]));
        std::cout << std::format("{:<28} {:>14.6e} {:>14.6e}\n",
                                 formatComplex(zeros[k], 6, true),
                                 residuals[k], trueResidual);
      }
  };

  /* 
  Coefficients coeff{
    {1.,0.},
    {-3,1.},
    {15.,0},
    {-3.5,0.},
    {5.,-1.},
    {5.,0.}
  };
  */
  // Seventh-degree polynomial
  Coefficients coeff{
    {1.,0.},
    {-3,0.},
    {15.,0},
    {-3.5,0.},
    {5.,-0.},
    {1.,-0.},
    {-3.,0.},
   {5.,0.}
	  };

  std::cout << "Polynomial:\n";
  std::cout << polynomialToString(coeff) << '\n';

  PolyHolder            holder(coeff);
  std::complex<double>  x = {1., 1.};
  auto const            y = polyEval(holder.pCoefficients(), x);
  std::cout << std::format("p({}) = {}\n", formatComplex(x), formatComplex(y));


  auto const der = holder.derivatives(x, 5);
  std::cout << std::format(
    "\nTesting derivatives at x = {}\n", formatComplex(x));
  std::cout << "Computed with synthetic division and from derivative "
               "coefficients\n";
  std::cout << std::format("{:<8} {:<28} {:<28}\n", "Order", "synt. div.",
                           "coeff.");
  std::cout << std::format("{:-<8} {:-<28} {:-<28}\n", "", "", "");
  unsigned int i{0u};
  for(auto const &v : der)
    {
      auto const coeffDer = holder.derivativeCoeff(i);
      auto const valDer = polyEval(coeffDer, x);
      std::cout << std::format("{:<8} {:<28} {:<28}\n", i,
                               formatComplex(v, 2, false, true),
                               formatComplex(valDer, 2, false, true));
      ++i;
    }

  std::cout << "\nTesting polynomial zeros\n";
  // To find complex zeros, the initial point must have a nonzero imaginary
  // part.

  auto [zeros, res, status] =
    polyRoots(coeff, coeff.size() - 1, std::complex<double>{0., 1.});

  printRootTable("Zeros and residuals", coeff, zeros, res);
  std::cout << std::format("Status: {}\n", status ? "success" : "failure");

  std::cout << "\nTesting polynomial zeros with complex coefficients:\n";
  coeff[1] = {2., -2.};
  coeff[2] = {0., -1.};
  std::cout << polynomialToString(coeff) << '\n';

  std::tie(zeros, res, status) =
    polyRoots(coeff, coeff.size() - 1, std::complex<double>{0., 1.});
  printRootTable("Zeros and residuals", coeff, zeros, res);
  std::cout << std::format("Status: {}\n", status ? "success" : "failure");

  std::cout << "\nNow a case with a multiple zero (equal to 1):\n";
  coeff.resize(4);
  coeff[0] = {-1., 0.};
  coeff[1] = {3., 0.};
  coeff[2] = {-3., 0.};
  coeff[3] = {1., 0.};
  std::cout << polynomialToString(coeff) << '\n';

  std::tie(zeros, res, status) =
    polyRoots(coeff, coeff.size() - 1, std::complex<double>{0., 1.});
  printRootTable("Zeros and residuals", coeff, zeros, res);
  std::cout << std::format("Status: {}\n", status ? "success" : "failure");
}
