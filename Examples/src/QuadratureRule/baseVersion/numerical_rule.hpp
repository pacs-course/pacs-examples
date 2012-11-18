#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP
#include "QuadratureRule.hpp"
namespace NumericalIntegration{
  class Simpson : public StandardQuadratureRule
  {
  public:
    Simpson();
    virtual QuadratureRuleHandler clone() const;
    virtual ~Simpson(){};
  };

  class MidPoint : public StandardQuadratureRule
  {
  public:
    MidPoint();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~MidPoint(){};
  };

  class Trapezoidal : public StandardQuadratureRule
  {
  public:
    Trapezoidal();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~Trapezoidal(){};
  };

}
#endif
