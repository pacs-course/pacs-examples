/*
 * testBasicZeroFun.cpp
 *
 *  Created on: Nov 21, 2020
 *      Author: forma
 */
#include "../basicOptimization.hpp"
#include <functional>
#include <iomanip>
#include <iostream>
int
main()
{
  // A function wit a minumum zero is known
  std::function<double(double)> fun;
  fun = [](double x) { return x * x * x * x - 1; };
  double a = 0;
  {
    auto [x1, x2, s] = apsc::bracketIntervalMinumum(fun, a);
    std::cout << "Bracket found:"
              << " a=" << x1 << " b=" << x2 << " with status=" << s << "\n";
    auto [res, status] = apsc::golden_search(fun, x1, x2);
    std::cout << "Golden Search result " << res << " Status= " << std::boolalpha
              << status << std::endl;
  }
}
