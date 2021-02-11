#include "Adams_rule.hpp"

namespace apsc::NumericalIntegration{

  
  Simpson::Simpson():StandardQuadratureRule<3>{
    {1./3,4./3,1./3},{-1.0,0.0,1.0},5
}
  {}
  
  std::unique_ptr<QuadratureRuleBase> Simpson::clone()const {
    return std::make_unique<Simpson>(*this);
  }
  
  
  MidPoint::MidPoint():StandardQuadratureRule<1>
  {
    {2.0},{0.0},3
    }
  {}

  std::unique_ptr<QuadratureRuleBase> MidPoint::clone() const {
    return std::make_unique<MidPoint>(*this);
  }

  Trapezoidal::Trapezoidal():StandardQuadratureRule<2>
  {
    {1.,1.},{-1.0,1.0},3
  }
  {}
  
  std::unique_ptr<QuadratureRuleBase> Trapezoidal::clone() const {
    return std::make_unique<Trapezoidal>(*this);
  }
  
}
