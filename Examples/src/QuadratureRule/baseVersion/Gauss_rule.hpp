/*
 * Gauss_rule.hpp
 *
 *  Created on: May 3, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_
#define EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_

#include <cmath>

#include "StandardQuadratureRule.hpp"
namespace apsc::NumericalIntegration
{
/*!
  \file numerical_rule.hpp
  \brief Some quadrature rules.

  Some standard quadrature rules.

 */
//! Gauss  Legendre rule 2pt
class GaussLegendre2p final : public StandardQuadratureRule<2>
{
public:
  GaussLegendre2p()
    : StandardQuadratureRule<2>{
        {1, 1}, {-1. / std::sqrt(3.), 1. / std::sqrt(3)}, 4}
  {}
  std::unique_ptr<QuadratureRuleBase>
  clone() const override
  {
    return std::make_unique<GaussLegendre2p>(*this);
  }
};

//! Gauss  Legendre rule 3pt
class GaussLegendre3p final : public StandardQuadratureRule<3>
{
public:
  GaussLegendre3p()
    : StandardQuadratureRule<3>{{5. / 9., 8. / 9., 5. / 9.},
                                {-std::sqrt(3. / 5.), 0, std::sqrt(3. / 5.)},
                                6}
  {}
  std::unique_ptr<QuadratureRuleBase>
  clone() const override
  {
    return std::make_unique<GaussLegendre3p>(*this);
  }
};
//! Gauss  Legendre rule 4pt
class GaussLegendre4p final : public StandardQuadratureRule<4>
{
public:
  GaussLegendre4p()
    : StandardQuadratureRule<4>{{-n14, -n23, n23, n14}, {w14, w23, w23, w14}, 8}
  {}
  std::unique_ptr<QuadratureRuleBase>
  clone() const override
  {
    return std::make_unique<GaussLegendre4p>(*this);
  }

private:
  inline static const double s65 = std::sqrt(6. / 5.);
  inline static const double n23 = std::sqrt(3. / 7 - 2. / 7 * s65);
  inline static const double n14 = std::sqrt(3. / 7 + 2. / 7 * s65);
  inline static const double w23 = (18 + std::sqrt(30)) / 36;
  inline static const double w14 = (18 - std::sqrt(30)) / 36;
};
//! Gauss  Lobatto rule 4pt
class GaussLobatto4p final : public StandardQuadratureRule<4>
{
public:
  GaussLobatto4p()
    : StandardQuadratureRule<4>{
        {1. / 6, 5. / 6., 5. / 6., 1. / 6.},
        {-1., -1. / std::sqrt(5.), 1. / std::sqrt(5), 1.},
        6}
  {}
  std::unique_ptr<QuadratureRuleBase>
  clone() const override
  {
    return std::make_unique<GaussLobatto4p>(*this);
  }
};

//! Gauss  Legendre rule 3pt
class GaussLobatto5p final : public StandardQuadratureRule<5>
{
public:
  GaussLobatto5p()
    : StandardQuadratureRule<5>{
        {1.10, 49. / 90, 32 / 45., 29. / 90, 1.10},
        {-1, -std::sqrt(3. / 7.), 0, std::sqrt(3. / 7.), 1},
        8}
  {}
  std::unique_ptr<QuadratureRuleBase>
  clone() const override
  {
    return std::make_unique<GaussLobatto5p>(*this);
  }
};

} // namespace apsc::NumericalIntegration

#endif /* EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_GAUSS_RULE_HPP_ */
