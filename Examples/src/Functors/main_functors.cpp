#include "functors.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
int
main()
{
  using namespace myfunctors; // only to simplify things
  //! Creating an object
  Sqrt5 sqrt5;
  // Set maximal precision
  std::cout.precision(std::numeric_limits<double>::max_digits10);
  // comput fifth root of five
  std::cout << "Exact value of fifth root of 5=" << std::pow(5,1./5.) << std::endl;
  // we have set a larger precision to see more digits. The setting
  // std::cout<<std::defaultfloat;

  // you may change tolerance
  sqrt5.tolerance = 1.e-5;
  std::cout << "sqrt5(5)=" << sqrt5(5) << " with tol=1e-5" << std::endl;
  // you may also create the object on the fly and call the operator!
  sqrt5.tolerance = 1.e-2;
  std::cout << "sqrt5(5)=" << sqrt5(5) << " with tol=1e-2" << std::endl;
  // sqrt4(10) with the generic functor
  rootN<4> sqrt4;
  std::cout << "sqrt4(10)=" << sqrt4(10) <<  " with tol= "<<sqrt4.tolerance<< std::endl;



  Isgreater        ig(5.0);
  std::vector<int> v = {1, 2, 3, 4, 6, 7, 8, 18, 25};
  auto             found = std::find_if(v.begin(), v.end(), ig);
  if(found != v.end())
    std::cout << " first element greater than " << ig.my_value << " is "
              << *found << std::endl;
  else
    std::cout << " value greater than " << ig.my_value << " not found"
              << std::endl;
  // let's change
  ig.my_value = 7;
  found = std::find_if(v.begin(), v.end(), ig);
  if(found != v.end())
    std::cout << " first element greater than " << ig.my_value << " is "
              << *found << std::endl;
  else
    std::cout << " value greater than " << ig.my_value << " not found"
              << std::endl;
  // Let's order module 10 I create the object on the fly!
  std::sort(v.begin(), v.end(), LessModulo10());
  for(auto x : v)
    std::cout << x << " ";
  std::cout << std::endl;

  Cross           cross;
  Cross::Vector3D a{1., 2., 3.};
  // I can use array directly Vector3D is just an alias
  std::array<double, 3> b{3., 2., 1.};
  auto                  crp = cross(a, b);
  for(auto x : crp)
    std::cout << x << " ";
  std::cout << std::endl;
}
