#include "numerical_rule.hpp"

namespace NumericalIntegration{

  
  Simpson::Simpson():StandardQuadratureRule<3>({{1./3,4./3,1./3}},{{-1.0,0.0,1.0}},5)
  {
  }
  
  QuadratureRuleHandler Simpson::clone()const { return 
      QuadratureRuleHandler(new Simpson(*this));}
  
  
  MidPoint::MidPoint():StandardQuadratureRule<1>({{2.0}},{{0.0}},3)		       
  {
  }

  QuadratureRuleHandler MidPoint::clone() const { 
    return QuadratureRuleHandler(new MidPoint(*this));
  }

  Trapezoidal::Trapezoidal():StandardQuadratureRule<2>({{1.,1.}},{{-1.0,1.0}},3)
  {
  }
  
  QuadratureRuleHandler Trapezoidal::clone() const { 
    return QuadratureRuleHandler(new Trapezoidal(*this));
  }
  
}
