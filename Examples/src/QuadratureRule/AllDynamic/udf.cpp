#include <cmath>
#include "udf.hpp"
extern "C"
{
  double fsincos(double const & x)
  {
    using namespace std;
    return sin(x)*cos(x);
  };
  double pippo(double const & x)
  {
    return log(1+fabs(x))*3*x*x-2*x;
    
  };
  double pluto(double const & x)
  {
    return x*x*std::sin(x);
  };
  
  double myfun(double const & x)
  {
    return 1;
  };
}


