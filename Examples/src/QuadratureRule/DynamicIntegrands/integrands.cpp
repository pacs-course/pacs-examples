#include "integrands.hpp"

extern "C"{

double fsincos(double const & x)
{
  using namespace std;
  return sin(x)*cos(x);
};

double square(double const& x)
{
  using namespace std;
  return x*x;
};

  double pippo(double const & x)
  {
  using namespace std;
  return x*std::exp(x);
  }

  double one(const double & x){return 1.0;}

  double foo(const double & x){return std::exp(x)*std::sin(x);}
  double toto(const double & x){return -2.0;}
}
