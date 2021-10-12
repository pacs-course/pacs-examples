#include "integrands.hpp"

double
fsincos(double const &x)
{
  using namespace std;
  return sin(x) * cos(x);
};

double
exact(double const &a, const double &b)
{
  using namespace std;
  return 0.5 * (sin(b) * sin(b) - sin(a) * sin(a));
};
