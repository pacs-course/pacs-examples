#include "numerical_rule.hpp"
#include "ruleProxy.hpp"
// Registration in the factory I use an unnamed namespace
namespace
{
  using namespace NumericalIntegration;
  using QuadratureRuleFactory::RuleProxy;
  
  RuleProxy<Simpson> SH("Simpson");
  
  RuleProxy<Trapezoidal> TH("Trapezoidal");
  
  RuleProxy<MidPoint> MH("MidPoint");
}
