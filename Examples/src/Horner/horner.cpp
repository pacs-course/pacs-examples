#include <cmath>
#include "horner.hpp"
#include <algorithm>
#include <cmath>
// Comment/uncomment next line if you dont want/want parallelization
// Test if I am compiling with g++ version >=9
#ifdef PARALLELEXEC
#if defined(__GNUC__) &&  (__GNUC__ >= 9)
#include<execution>
#else
#undef PARALLELEXEC
#endif
#endif

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
  for (std::size_t k = 1; k<a.size(); ++k){ 
    sum += a[k]*integerPow(x,k);// Pow is expensive
    // If you want to test with the standard pow comment the
    // previous statement and uncomment the next one
    //sum += a[k]*std::pow(x,k);// Pow is expensive
  }
  return sum;
}

double  horner(std::vector<double> const & a, double const & x){
  double u = a.back();// last value
  for (auto  i=a.crbegin()+1;i!=a.crend();++i)
    u = u*x + *i;
  return u;
}

#ifdef PARALLELEXEC
#warning "Using parallel implementation of std::transform"
//! Evaluates polynomial in a set of points (parallel version)
std::vector<double>
evaluatePoly(std::vector<double> const & points,
		  std::vector<double> const & a,
                  polyEval method)
{
  std::vector<double> result(points.size());
  // if you prefer a normal loop
  // for (std::size_t i=0;i<points.size();++i) result[i]=method(a,points[i]);
  // Here I use std::transform to have parallelism "for free"
  auto compute=[&a,&method] (double const & x){return method(a,x);};
  std::transform(std::execution::par, points.begin(),points.end(),
                 result.begin(),compute);
  return result;
}
#else
#warning "Using sequential implementation of std::transform"
//! Evaluates polynomial in a set of points
std::vector<double>
evaluatePoly(std::vector<double> const & points,
		  std::vector<double> const & a,
                  polyEval method)
{
  std::vector<double> result(points.size());
  // if you prefer a normal loop
  // for (std::size_t i=0;i<points.size();++i) result[i]=method(a,points[i]);
  // Here I use std::transform to be consistent with the parallel version
  auto compute=[&a,&method] (double const & x){return method(a,x);};
  std::transform(points.begin(),points.end(),result.begin(),compute);
  return result;
}
#endif

