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
  class Simpson : public StandardQuadratureRule<3>
  {
  public:
    Simpson();
    virtual QuadratureRuleHandler clone() const;
    virtual ~Simpson(){};
  };

  //! Midpoint rule
  class MidPoint : public StandardQuadratureRule<1>
  {
  public:
    MidPoint();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~MidPoint(){};
  };

  //! Trapezoidal rule
  class Trapezoidal : public StandardQuadratureRule<2>
  {
  public:
    Trapezoidal();
    virtual QuadratureRuleHandler clone()const ;
    virtual ~Trapezoidal(){};
  };

}
#endif
