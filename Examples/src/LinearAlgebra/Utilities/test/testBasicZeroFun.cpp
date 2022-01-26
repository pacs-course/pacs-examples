/*
 * testBasicZeroFun.cpp
 *
 *  Created on: Nov 21, 2020
 *      Author: forma
 */
#include "../basicZeroFun.hpp"
#include <functional>
#include <iostream>
int
main()
{
  // A function whose zero is known
  std::function<double(double)> fun;
  fun = [](double x) { return x * x * x - 1; };
  double a = 0;
  double b = 3;
  {
    auto res = apsc::bisection(fun, a, b, 1.e-4);
    std::cout << "Bisection result " << res << std::endl;
  }
  {
    auto res = apsc::regulaFalsi(fun, a, b, 1.e-4);
    std::cout << "regulaFalsi result " << res << std::endl;
  }
  {
    b = -1;
    auto [res, status] = apsc::secant(fun, a, b);
    std::cout << "Secant result " << res << " Status" << status << std::endl;
  }
  {
    a=-1;
    auto df=[](double x) { return 3*x * x; };
    auto [res, status] = apsc::Newton(fun, df, a);
    std::cout << "Newton result " << res << " Status" << status << std::endl;
  }

  {
    b = -1;
    auto [x1, x2, s1] = apsc::bracketInterval(fun, a);
    std::cout << "Bracket found:"
              << " a=" << x1 << " b=" << x2 << " with status=" << s1 << "\n";
    auto res = apsc::bisection(fun, x1, x2, 1.e-4);
    std::cout << "Bisection result " << res << std::endl;
  }
  {
    b = -1;
    auto [x1, x2, s1] = apsc::bracketInterval(fun, a);
    std::cout << "Bracket found:"
              << " a=" << x1 << " b=" << x2 << " with status=" << s1 << "\n";
    auto [res, status] = apsc::brent_search(fun, x1, x2, 1.e-4);
    std::cout << "Brent result " << res << std::endl;
  }

}
