#include "Adams_rule.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "montecarlo.hpp"
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

  RuleProxy<Simpson> SH("Simpson");
  RuleProxy<Trapezoidal> TH("Trapezoidal");
  RuleProxy<MidPoint> MH("MidPoint");
  RuleProxy<QuadratureRuleAdaptive<Simpson>> AD("Adaptive");
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
  addRuleToFactory<Simpson>("Simpson");
  addRuleToFactory<Trapezoidal>("Trapezoidal");
  addRuleToFactory<MidPoint>("MidPoint");
  addRuleToFactory<QuadratureRuleAdaptive<Simpson>>("AdaptiveSimpson");
}
} // namespace
