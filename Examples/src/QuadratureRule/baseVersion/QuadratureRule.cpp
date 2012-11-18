#include "QuadratureRule.hpp"
#include <algorithm>
double 
NumericalIntegration::StandardQuadratureRule::apply(FunPoint const & f)const
{
  double tmp(0);
  auto np=_n.begin();
  for (auto wp=_w.begin();wp<_w.end();++wp,++np)tmp+=f(*np)*(*wp);
  return tmp;
}
