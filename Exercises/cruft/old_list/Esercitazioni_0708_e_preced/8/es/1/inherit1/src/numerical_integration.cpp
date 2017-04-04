#include "numerical_integration.hpp"

namespace NumericalIntegration{
  
  QuadratureBase::QuadratureBase(double const & a,
				 double const & b,
				 int const & nint):
    _a(a),
    _b(b),
    _h((b-a)/(double) nint),
    _nint(nint),
    _knots(nint+1)
  {
    _knots[0]=_a;
    for(int i=1;i<_knots.size();++i)_knots[i]=_knots[i-1]+_h;
  }
  
  
  
  Simpson::Simpson(double const & a, double const & b, int const & nint):
    QuadratureBase(a,b,nint)
  {}


  double Simpson::apply(FunPoint f) const
  {
    double result(0);
    double nl[3];
    const double w=_h/6.0;
    for(int i=0;i<_knots.size()-1;++i){
      nl[0]  = _knots[i  ];
      nl[2]  = _knots[i+1];
      nl[1]  = 0.5*(nl[0]+nl[2]);
      result+= w*(f(nl[0])+4*f(nl[1])+f(nl[2]));
    }
    return result;
  }


  MidPoint::MidPoint(double const & a, double const & b, int const & nint):
    QuadratureBase(a,b,nint){}


  double MidPoint::apply(FunPoint f) const
  {
    double result(0);
    for(int i=0;i<_knots.size()-1;++i)
      result+=_h*( f(0.5*(_knots[i]+_knots[i+1])));
    return result;
  }


}
