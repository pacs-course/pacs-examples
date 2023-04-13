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
} // namespace apsc::NumericalIntegration

#endif /* EXAMPLES_SRC_QUADRATURERULE_BASEVERSION_QUADRATURERULETRAITS_HPP_ */
