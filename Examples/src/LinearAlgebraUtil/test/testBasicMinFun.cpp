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
    auto [x1, x2, s] = apsc::bracketIntervalMinimum(fun, a);
    std::cout << "Bracket found:"
              << " a=" << x1 << " b=" << x2 << " with status=" << s << "\n";
    {
    auto [res, status] = apsc::golden_search(fun, x1, x2);
    std::cout << "Golden Search result " << res << " Status= " << std::boolalpha
              << status << std::endl;
    }
    {
    auto [res, fval] = apsc::Brent_glomin(x1, x2,x1,1,1e-6,1e-6,fun);
    std::cout << "Brent_glomin result " << res << " functionvalue= "
              << fval << std::endl;
    }
    {
    auto [res, fval] = apsc::Brent_local_min(x1, x2,1e-6,fun);
    std::cout << "Brent_local_min result " << res << " functionvalue= "
              << fval << std::endl;
    }
  }
}
