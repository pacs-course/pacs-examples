#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP
#include "StandardQuadratureRule.hpp"
namespace apsc::NumericalIntegration
{
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
  std::unique_ptr<QuadratureRuleBase> clone() const override;
};

//! Midpoint rule
class MidPoint final : public StandardQuadratureRule<1>
{
public:
  MidPoint();
  std::unique_ptr<QuadratureRuleBase> clone() const override;
};

//! Trapezoidal rule
class Trapezoidal final : public StandardQuadratureRule<2>
{
public:
  Trapezoidal();
  std::unique_ptr<QuadratureRuleBase> clone() const override;
};

} // namespace apsc::NumericalIntegration
#endif
