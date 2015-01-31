#include <cmath>
#include "horner.hpp"
double eval (std::vector<double> const & a, double const x){  
  double sum = a[0];
  for (std::vector<double>::size_type k = 1; k<a.size(); ++k){ 
    sum += a[k]*pow(x,k);// Pow is VERY expensive
  }
  return sum;
}

double  horner(std::vector<double> const & a, double const x){
  double u = a.back();// last value
  for (std::vector<double>::size_type i = a.size()-1 ; i != -1; --i) 
    u = u*x + a[i];
  return u;
}

//! Evaluates polynomial in a set of points
void evaluatePoly(std::vector<double> const & points,
		  std::vector<double> const & a,
		  std::vector<double>  & result, polyEval method)
{
  
  result.clear();// clean previous results
  result.reserve(points.size()); // make sure there is space
  for(std::vector<double>::const_iterator 
	i=points.begin();i!=points.end();++i) 
    result.push_back(method(a,*i));
  // C++11
  /*
  for(auto i : points)points.push_back(method(a,i));
  */

  return;
}



