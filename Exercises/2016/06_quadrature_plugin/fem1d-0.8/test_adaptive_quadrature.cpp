#include <iostream>
#include <iomanip>
#include <cmath>
#include <dlfcn.h>
#include <functional>

double
integrand (double x)
{ return (pow (sin (pow (x, 2)), 2)); }

int main ()
{
  double (*integrate) (std::function<double (double)>, double, double);
  
  void * handle = dlopen ("adaptive_quadrature.so", RTLD_LAZY);
  if (! handle)
    {
      std::cerr << "cannot load object!" << std::endl;
      std::cerr << dlerror () << std::endl;
      return (-1);
    }

  void * sym = dlsym (handle, "integrate");
  if (! sym)
    {
      std::cerr << "cannot load symbol!" << std::endl;
      std::cerr << dlerror () << std::endl;
      return (1);
    }
  
  integrate = reinterpret_cast<double (*) (std::function<double (double)>, double, double)> (sym);
  
  double pi = 4 * atan (1);
  double res = integrate (integrand, 0, pi);
  return 0;
}
