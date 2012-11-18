#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP
#include "QuadratureRule.hpp"
namespace NumericalIntegration{
  /*!
    \file numerical_rule.hpp 
    \brief Some quadrature rules.
    
    Some standard quadrature rules.
    
   */
  //! Simpson rule
  class Simpson : public StandardQuadratureRule
  {
  public:
    Simpson();
    virtual QuadratureRuleHandler clone() const;
    virtual ~Simpson(){};
  };

  //! Midpoint rule
  class MidPoint : public StandardQuadratureRule
  {
  public:
    MidPoint();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~MidPoint(){};
  };

  //! Trapezoidal rule
  class Trapezoidal : public StandardQuadratureRule
  {
  public:
    Trapezoidal();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~Trapezoidal(){};
  };

}
#endif
