#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include "GetPot"
#include <cstdlib>
#if defined(SINGLE_PRECISION)
using Real=float;
std::string filename("single.dat");
#elif defined(EXTENDED_PRECISION)
using Real=long double;
std::string filename("extended.dat");
#else
using Real=double;
std::string filename("double.dat");
#endif
const Real a=100.0;
const Real b=4.0;

//! \f$ a e^{bx}\f$
Real fun (Real const & x)
{
  return a*std::exp(b*x);
} 

//! Derivative of \f$ a e^{bx}\f$
Real dfun (Real const & x)
{
  return a*b*std::exp(b*x);
} 

//! Tests of accuracy of finite differences
/*!
  It computes \f$ \frac{df}{dx}(x)\f$ at a given point x using second order
  centered finite differences. It outputs the error. The example wants to show that 
  floating point truncation error finally dominates.
 */
int main()
{
  // Get roundoff unit
  const Real u=0.5*std::numeric_limits<Real>::epsilon();
  std::cout<<"Roundoff unit="<<u<<std::endl;
  // Read some data
  GetPot   ifl("data.pot");
  // Initial spacing
  Real h = ifl("h", 0.1);
  std::cout<<" Initial Spacing "<<h<<std::endl;
  // Number of subdivisions
  const unsigned int n = ifl("n",10);
  std::cout<<" Number of subdivisions "<<n<<std::endl;
  // Point where derivative is computed
  const Real x = ifl("x",3.0);
  std::cout<<" X value "<<x<<std::endl;
  
  std::vector<Real> derNumer;
  std::vector<Real> der4Numer;
  std::vector<Real> Error;
  std::vector<Real> Error4;
  std::vector<Real> spacing;
  std::vector<Real> truncationErrorEstimate;
  derNumer.reserve(n);
  der4Numer.reserve(n);
  Error.reserve(n);
  Error4.reserve(n);
  spacing.reserve(n);
  truncationErrorEstimate.reserve(n);
  Real constexpr half(0.5);
  Real constexpr twelvth(static_cast<Real>(1)/static_cast<Real>(12));
  Real constexpr twothird(static_cast<Real>(2)/static_cast<Real>(3));
  
  for (int i=0;i<n;++i)
    {
      spacing.push_back(h);
      // Second order formula
      Real dn = half*(fun(x+h)-fun(x-h))/h;
      // Fourth order formula
      Real dn4= (twelvth*fun(x-2*h)-twothird*fun(x-h)-twelvth*fun(x+2*h)+
                 twothird*fun(x+h))/h;
      Real de = dfun(x);
      derNumer.push_back(dn);
      der4Numer.push_back(dn4);
      Error.push_back(std::abs(de-dn));
      Error4.push_back(std::abs(de-dn4));
      truncationErrorEstimate.push_back(u*dfun(x)*x/h); 
      h/=2.;
    }
  // Write data
  std::ofstream file(filename.c_str());
  for (int i=0;i<n;++i)
    {
      file<<spacing[i]<<" "<<derNumer[i]<<" "<<Error[i]<<" "<<truncationErrorEstimate[i]<<" "<<Error4[i]<<std::endl;
    }
  file.close();
}
