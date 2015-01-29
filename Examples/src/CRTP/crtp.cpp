#include "crtp.hpp"
double
Derived1::fun(double const & x)
{
  return x;
}

double
Derived2::fun(double const & x)
{
  return x*x;
}
