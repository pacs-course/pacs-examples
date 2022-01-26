/*
 * test_mathUtils.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "Eigen/Core"
#include "mathUtils.hpp"
#include <complex>
#include <iostream>
#include <vector>
int
main()
{
  using namespace apsc::TypeTraits;
  double                            a{3.0};
  double                            b{5.0};
  std::complex<double>              c1{1., 2.};
  std::complex<double>              c2{3., 4.};
  std::vector<double>               v1{1., 2., 3.};
  std::vector<double>               v2 = {3., 4., 5.};
  std::vector<std::complex<double>> vc1{{1., 2.}, {3., 4}};
  std::vector<std::complex<double>> vc2 = {{3., 4.}, {5., 6}};
  Eigen::Vector3d                   e1;
  Eigen::Vector3d                   e2;
  e1 << 1., 2., 3.;
  e2 << 3., 4., 5.;
  std::cout << " dot of two doubles " << apsc::math_util::dot(a, b)
            << std::endl;
  std::cout << " dot of two vectors " << apsc::math_util::dot(v1, v2)
            << std::endl;
  std::cout << " dot of two Eigen vectors " << apsc::math_util::dot(e1, e2)
            << std::endl;
  std::cout << " dot of two complex " << apsc::math_util::dot(c1, c2)
            << std::endl;
  std::cout << " dot of two complex vectors " << apsc::math_util::dot(vc1, vc2)
            << std::endl;

  std::cout << " normInf of a double " << apsc::math_util::normInf(a)
            << std::endl;
  std::cout << " normInf of a vectors " << apsc::math_util::normInf(v1)
            << std::endl;
  std::cout << " normInf of a Eigen vector " << apsc::math_util::normInf(e1)
            << std::endl;
  std::cout << " normInf of a complex  " << apsc::math_util::normInf(c1)
             << std::endl;
  std::cout << " normInf of a complex vector " << apsc::math_util::normInf(vc2)
            << std::endl;

  std::cout << " norm2 of a double " << apsc::math_util::norm2(a)
             << std::endl;
   std::cout << " norm2 of a vectors " << apsc::math_util::norm2(v1)
             << std::endl;
   std::cout << " norm2 of a Eigen vector " << apsc::math_util::norm2(e1)
             << std::endl;
   std::cout << " norm2 of a complex " << apsc::math_util::norm2(c1)
              << std::endl;
   std::cout << " norm2 of a complex vector " << apsc::math_util::norm2(vc2)
                   << std::endl;


   using namespace apsc::math_util::vectorOperators;
   std::cout<<"First  vector="<<v1<<std::endl;
   std::cout<<"Second vector="<<v2<<std::endl;
   std::cout<<"Sum          ="<<(v2+v1)<<std::endl;
   std::cout<<"Difference   ="<<(v1-v2)<<std::endl;
   std::cout<<"First vector * 5  ="<<(v1*5.)<<std::endl;
   std::cout<<"Distance   ="<<apsc::math_util::squaredDistance(v1,v2)<<std::endl;

}
