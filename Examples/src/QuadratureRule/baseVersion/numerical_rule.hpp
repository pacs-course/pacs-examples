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
  class Simpson final : public StandardQuadratureRule<3>
  {
  public:
    Simpson();
    QuadratureRuleHandler clone() const;
  };

  //! Midpoint rule
  class MidPoint final : public StandardQuadratureRule<1>
  {
  public:
    MidPoint();
    QuadratureRuleHandler clone()const ;
  };

  //! Trapezoidal rule
  class Trapezoidal final : public StandardQuadratureRule<2>
  {
  public:
    Trapezoidal();
    QuadratureRuleHandler clone()const ;
  };

}
#endif
