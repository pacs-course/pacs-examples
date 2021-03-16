/*!
 * \file main_Richardson.cpp
 *
 * Here I use the Richardson extrapolation following the example in Wikipedia.
 * I want to solve
 *
 *\f[
 * y^{\prime}(t)=-y^2(t), \quad t\in (0,5]
 *\f]
 * with \f$y(0)=1$
 * I solve with the trapezoidal rule (Crank Nicolson) that has an error of type
 * \f[
 * c_0 h^2 + c_1 h^4 + \ldots + c_k h^{2(k+1} + \ldots
 * \f]
 */
#ifndef HH_CRANKNICOLSON_HPP
#define HH_CRANKNICOLSON_HPP
#include "Richardson.hpp"
#include "basicZeroFun.hpp"
#include <iostream>
#include <tuple>
#include <vector>
// Function f(y,t)
namespace apsc
{
template <class Function>
double
CrankNicolsonStep(Function const &f, double y0, double t0, double h)
{
  auto const fn = f(y0, t0);
  auto       fStep = [&](const double &y) {
    return y - 0.5 * h * (f(y, t0 + h) + fn) - y0;
  };
  //    auto[ya,yb,status]=apsc::bracketInterval(fStep, y0, h);
  //    if(status==false) std::cerr<<" Cannot bracket zero\n";
  double yb = y0 + h*fn;
  auto [y1, status2] = apsc::secant(fStep, y0, yb, 1.e-8, 1.e-10, 200);
  if (status2 == false)
    {
      std::cerr << " Cannot find solution with h=" << h << "\n";
      std::tie(y1, status2) = apsc::secant(fStep, y0, y0+0.01*h*fn, 1.e-8, 1.e-10, 200);
      if (status2 == false)
        std::cerr << " Failed also at second attempt! Reduce h!\n";
    }
  return y1;
}

template <class Function>
std::tuple<std::vector<double>, std::vector<double>>
CrankNicolson(Function const &f, double y0, double t0, double T, unsigned int n)
{
  auto                h = (T - t0) / n;
  std::vector<double> y;
  std::vector<double> t;
  y.reserve(n + 1);
  t.reserve(n + 1);
  y.push_back(y0);
  t.push_back(t0);
  for (std::size_t i = 0; i < n; ++i)
    {
      auto yn = CrankNicolsonStep(f, y.back(), t.back(), h);
      y.push_back(yn);
      t.push_back(t.back() + h);
    }
  return {t, y};
}
} // and namespace apcs
#endif
