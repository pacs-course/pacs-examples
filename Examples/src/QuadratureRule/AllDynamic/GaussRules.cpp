// #include "Adams_rule.hpp"
#include "Gauss_rule.hpp"
#include "QuadratureRuleAdaptive.hpp"
// #include "montecarlo.hpp"
#include "ruleFactory.hpp"
// uncomment the next include if you want to use
// #include "ruleProxy.hpp"
//  NEW SIMPLIFIED VERSION
/*!\file myrules.cpp
  In this file I define the quadrature rules that I want to put in the library
  that loads them into the Factory/

 */
//! Registration in the factory I use an unnamed namespace
/*!
  When I load the library I must be sure that the symbols are loaded
  so I need to use the option -Wl,-E or -export-dynamic
 */
/*
namespace
{
  using namespace apsc::NumericalIntegration;
  using apsc::QuadratureRuleFactory::RuleProxy;
  RuleProxy<QuadratureRuleAdaptive<GaussLegendre3p>> AD2("AdaptiveGauss3");
  RuleProxy<GaussLegendre3p> GL3("GaussLegendre3");
  RuleProxy<GaussLegendre2p> GL2("GaussLegendre2");
  RuleProxy<GaussLobatto4p> gL4("GaussLobatto4");
  RuleProxy<GaussLobatto5p> gL5("GaussLobatto5");
}
*/
//! Alternative use a function.
/*
    You must define with constructor attribute
*/

//! Another alternative. The function is automatically loaded

namespace
{
__attribute__((constructor)) void
loadFactoryItems()
{
  using namespace apsc::QuadratureRuleFactory;
  using namespace apsc::NumericalIntegration;
  addRuleToFactory<QuadratureRuleAdaptive<GaussLegendre3p>>("AdaptiveGauss3");
  addRuleToFactory<QuadratureRuleAdaptive<GaussLegendre4p>>("AdaptiveGauss4");
  addRuleToFactory<GaussLobatto4p>("GaussLobatto4");
  addRuleToFactory<GaussLobatto5p>("GaussLobatto5");
  addRuleToFactory<GaussLegendre3p>("GaussLegendre3");
  addRuleToFactory<GaussLegendre2p>("GaussLegendre2");
  addRuleToFactory<GaussLegendre4p>("GaussLegendre4");
}
} // namespace
