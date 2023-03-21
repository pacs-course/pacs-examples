#include "mean.hpp"
#include <eigen3/Eigen/Dense>
#include <complex>
#include <iostream>
#include <string>
int
main()
{
  using namespace Eigen;
  using namespace Utility;

  constexpr int i = 42;
  std::cout << "mean(7,i):   " << mean(7, i) << std::endl;
  double f1 = 3.4;
  double f2 = -6.7;
  std::cout << "mean(f1,f2): " << mean(f1, f2) << std::endl;
  Matrix2d a;
  a << 1, 2, 3, 4;
  Matrix2d b(2, 2);
  b << 2, 3, 1, 4;
  std::cout << "mean(a,b): " << mean(a, b) << std::endl;

  std::cout << "mean('a','z'): " << mean('a', 'z') << std::endl;

  double *ap = new double(5.0);
  double *bp = new double(6.0);
  std::cout << "mean('ap','bp'): " << mean(ap, bp) << std::endl;
  {
    std::cout << " Now with vectors of int" << std::endl;
    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{1, 1, 1};
    auto             vres = mean(v1, v2);
    for(auto i : vres)
      std::cout << i << " ";
  }
  std::cout << std::endl;
  {
     std::cout << " Now with arrays of double" << std::endl;
     std::array v1{1., 2., 3.};
     std::array v2{1., 1., 1.};
     auto             vres = mean(v1, v2);
     for(auto i : vres)
       std::cout << i << " ";
   }


  std::cout << std::endl;
  // Now Sum and Mean
  std::cout << Sum(3, 4.5, 6.7, 1.0, 8, 9, 10) << std::endl;
  std::string hello{"Hello"};
  std::string world{"world"};
  std::string space{" "};
  std::cout << Sum(hello, space, world) << std::endl;
  std::complex<double> one(1.0, 0.);
  std::complex<double> im(0., 1.0);
  std::complex<double> biz(1.0, 1.0);
  std::cout << Sum(one, im, biz, 10.0) << std::endl;
  std::cout << Mean(1.5, 1., 1., 1) << std::endl;
  std::cout << Mean(im, biz, one, 1.0, 2.0) << std::endl;
}
