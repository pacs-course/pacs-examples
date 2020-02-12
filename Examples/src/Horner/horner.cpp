#include <cmath>
#include "horner.hpp"
//! My function for power
/*!
 Since std::pow() is very expensive I create my version for integer
 powers. 
 */
double integerPow(const double & x, int n)
{
  double res{x};
  for (int i=2;i!=n+1;++i)res*=x;
  return res;
}

double eval (std::vector<double> const & a, double const & x){  
  double sum = a[0];
  for (std::vector<double>::size_type k = 1; k<a.size(); ++k){ 
    sum += a[k]*integerPow(x,k);// Pow is VERY expensive
  }
  return sum;
}

double  horner(std::vector<double> const & a, double const & x){
  double u = a.back();// last value
  for (auto  i=a.crbegin()+1;i!=a.crend();++i)
    u = u*x + *i;
  return u;
}

//! Evaluates polynomial in a set of points
std::vector<double>
evaluatePoly(std::vector<double> const & points,
		  std::vector<double> const & a,
                  polyEval method)
{
  std::vector<double> result; 
  result.reserve(points.size()); // make sure there is space
  for(auto i : points)result.push_back(method(a,i));
  return result;
}



