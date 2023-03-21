/*
 * mainVariadic.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */
#include "folding.hpp"
#include <set>
#include <string>
#include <vector>
int
main()
{
  using namespace apsc;
  std::cout << "Testing fold expressions\n";
  double *p = nullptr; // a null puntier converts to false
  bool    yes = true;
  int     j = 0; // An int converts to bool. 0 means false
  std::cout << std::boolalpha << " Are all true?" << allTrue(j, yes, true, p)
            << std::endl;
  ;
  double x = 10;
  p = &x; // a non null pointer maps to true
  j = 2;  // an int different from 0 maps to true
  std::cout << std::boolalpha << " Are all true?" << allTrue(j, yes, true, p)
            << std::endl;

  auto res = allSum(x, 3, 6, 8.5, -3);
  std::cout << " The sum is " << res << std::endl;
  std::cout << " The mean is " << apsc::allMean(x, 3, 6, 8.5, -3) << std::endl;

  std::cout << " The product is " << apsc::allProd(x, 3, 6, 8.5, -3)
            << std::endl;

  std::string a1{"This "};
  std::string a2{"is "};
  std::string a3{"the "};
  std::string a4{"sum "};
  std::string a5{"of 5 strings\n "};
  std::cout << allSum(a1, a2, a3, a4, a5);


}
