#include "numerical_integration.hpp"
#include <iostream>

namespace NumericalIntegration{
  
  Quadrature::Quadrature(QuadratureRule & rule,
			 double const & a,
			 double const & b,
			 int const & nint):
    _rule(rule),
    _a(a),
    _b(b),
    _h((b-a)/(double) nint),
    _wf(_h/2.0),
    _nint(nint),
    _knots(nint+1)
  {
    _knots[0]=_a;
    for(int i=1;i<_knots.size();++i)_knots[i]=_knots[i-1]+_h;
  }
  
  double Quadrature::apply(FunPoint f)
  {
    double result(0);
    int nnodes=_rule.num_nodes();
    for(int i=0;i<_knots.size()-1;++i){
      double a=_knots[i];
      double b=_knots[i+1];
      double xm=(a+b)/2.0;
      for (int j=0;j<nnodes;++j){
	double x=xm+_wf*_rule.node(j);
	result+= _wf*f(x)*_rule.weight(j);
      }
    }
    return result;
  }

}
