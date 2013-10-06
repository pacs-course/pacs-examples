#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include "GetPot"
#include <cstdlib>
double fun (double const & x)
{
  return 100.*std::exp(x);
} 

double dfun (double const & x)
{
  return 100.*std::exp(x);
} 

// Tests accuracy of finite difference
int main()
{
  // Get roundoff unit
  const double u=0.5*std::numeric_limits<double>::epsilon();
  GetPot   ifl("data.pot");
  // Initial spacing
  double h = ifl("h", 0.1);
  std::cout<<" Initial Spacing "<<h<<std::endl;
  // Number of subdivisions
  const unsigned int n = ifl("n",10);
  std::cout<<" Number of subdivisions "<<n<<std::endl;
  // Point where derivative is computed
  const double x = ifl("x",3.0);
  std::cout<<" X value "<<x<<std::endl;
  
  std::vector<double> derNumer;
  std::vector<double> Error;
  std::vector<double> spacing;
  std::vector<double> truncationErrorEstimate;
  derNumer.reserve(n);
  Error.reserve(n);
  spacing.reserve(n);
  truncationErrorEstimate.reserve(n);
  for (int i=0;i<n;++i)
    {
      spacing.push_back(h);
      double dn = 0.5*(fun(x+h)-fun(x-h))/h;
      double de = dfun(x);
      derNumer.push_back(dn);
      Error.push_back(std::abs(de-dn));
      truncationErrorEstimate.push_back(u*dfun(x)*x/h); 
      h=h/2;
    }
  // Write data
  std::ofstream file("output.dat");
  for (int i=0;i<n;++i)
    {
      file<<spacing[i]<<" "<<derNumer[i]<<" "<<Error[i]<<" "<<truncationErrorEstimate[i]<<std::endl;
    }
  file.close();
}
