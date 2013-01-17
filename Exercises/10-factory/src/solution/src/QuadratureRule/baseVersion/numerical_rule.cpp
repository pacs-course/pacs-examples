#include "numerical_rule.hpp"

namespace NumericalIntegration{


  Simpson::Simpson(){
    _w.push_back(1./3);
    _w.push_back(4./3);
    _w.push_back(1./3);
    _n.push_back(-1.0);
    _n.push_back(0.0);
    _n.push_back(1.0);
    my_order=5;
  }

  QuadratureRuleHandler Simpson::clone()const { return 
      QuadratureRuleHandler(new Simpson(*this));}

  
  MidPoint::MidPoint(){
    _w.push_back(2.0);
    _n.push_back(0.0);
    my_order=3;
  }

  QuadratureRuleHandler MidPoint::clone() const { 
    return QuadratureRuleHandler(new MidPoint(*this));
  }

  Trapezoidal::Trapezoidal(){
    _w.push_back(1.0);
    _w.push_back(1.0);
    _n.push_back(-1.0);
    _n.push_back(1.0);
    my_order=3;
  }
  
  QuadratureRuleHandler Trapezoidal::clone() const { 
    return QuadratureRuleHandler(new Trapezoidal(*this));
  }
  
}
