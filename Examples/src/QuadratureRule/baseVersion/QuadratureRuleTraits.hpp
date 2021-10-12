/*
 * QuadratureRuleTraits.hpp
 *
 *  Created on: Feb 10, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_QUADRATURERULETRAITS_HPP_
#define EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_QUADRATURERULETRAITS_HPP_
#include "CloningUtilities.hpp"
#include <functional>
#include <memory>
// This is a simple trait, just in a namespace
namespace apsc::NumericalIntegration
{
//! The type the integrand
using FunPoint = std::function<double(double const &)>;

//! forward declaration
class QuadratureRuleBase;
//! The type of the object holding the quadrature rule.
// using QuadratureRuleHandler=std::unique_ptr<QuadratureRule>;
//! I am using the PoiterWrapper for clonable classes defined in
//! CloningUtilities.hpp
using QuadratureRuleHandler = apsc::PointerWrapper<QuadratureRuleBase>;
} // namespace apsc::NumericalIntegration

#endif /* EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_QUADRATURERULETRAITS_HPP_ */
