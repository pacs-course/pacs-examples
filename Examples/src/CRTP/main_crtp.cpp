#include "MyMat0.hpp"
#include "crtp.hpp"
#include "point.hpp"
#include <iostream>
// A function that calls Base<T>.fun
template <class T>
double
callFun5(Base<T> const &a)
{
  return a.fun(5.0);
}
int
main()
{
  using std::cout;
  using std::endl;
  Derived1 d1;
  Derived2 d2;
  cout << "Derived" << d1.fstatic() << " Derived1::fun(5.0)=" << callFun5(d1)
       << endl;
  cout << "Derived" << d2.fstatic() << " Derived2::fun(5.0)=" << callFun5(d2)
       << endl;

  // now a point
  apsc::crtp::Point<3> a{{1.0, 2.0, 3.0}};
  apsc::crtp::Point<3> b{{1.0, 2.0, 3.0}};

  cout << " Is a different from b? " << std::boolalpha << (a != b) << endl;

  // Now the matrix
  // Create a random matrix
  LinearAlgebra::MyMat0 M(3, 4);
  M.fillRandom();
  cout << " Element (2,3)=" << M(2, 3) << " Element [2][3]=" << M[2][3] << endl;
  M[2][3] = 1234.56;
  cout << " Element (2,3)=" << M(2, 3) << " Element [2][3]=" << M[2][3] << endl;
}
