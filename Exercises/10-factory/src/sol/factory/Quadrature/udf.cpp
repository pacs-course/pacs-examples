#include <cmath>
#include "udf.hpp"
#include "udfProxy.hpp"

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

// Registrazione
namespace
{
  UDFProxy FSINCOS(fsincos, "fsincos");
  
  UDFProxy PIPPO(pippo, "pippo");
  
  UDFProxy PLUTO(pluto, "pluto");

  UDFProxy MYFUN(myfun, "myfun");
}
