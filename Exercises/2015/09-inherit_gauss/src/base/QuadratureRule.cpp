#include "QuadratureRule.hpp"
#include <algorithm>
double 
NumericalIntegration::StandardQuadratureRule::apply(
						    FunPoint const &f, 
						    double const &a, 
						    double const &b)const
{
  double h2((b-a)*0.5); // half length
  double xm((a+b)*0.5); // midpoint
  auto fscaled=[&](double x){return f(x*h2+xm);};
  double tmp(0);
  auto np=_n.begin();
  for (auto wp=_w.begin();wp<_w.end();++wp,++np)tmp+=fscaled(*np)*(*wp);
  return h2*tmp;
}
