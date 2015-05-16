#include "numerical_rule.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "ruleProxy.hpp"
//! Registration in the factory I use an unnamed namespace
/*!
  When I load the library I must be sure that the symbols are loaded
  so I need to use the option -Wl,-E or -Wl,-export-dynamic
 */
namespace TheRules
{
  using namespace NumericalIntegration;
  using QuadratureRuleFactory::RuleProxy;
  
  RuleProxy<Simpson> SH("Simpson");
  
  RuleProxy<Trapezoidal> TH("Trapezoidal");
  
  RuleProxy<MidPoint> MH("MidPoint");
  
  RuleProxy<QuadratureRuleAdaptive<Simpson>> AD("Adaptive");

}

//!Alternative use a function.
/*! You must load the function and run it.
  @code
    void * dylib=dlopen(libraryname,RTLD_NOW);
    void * lfi = dlsym(dylib,"loadFactoryItems")
    static_cast<void(*)()>(lfi)();
  @endcode
 */
extern "C"
{
  void loadFactoryItems()
  {
    using namespace NumericalIntegration;
    RuleProxy<Simpson> SH("Simpson");
    RuleProxy<Trapezoidal> TH("Trapezoidal");
    RuleProxy<MidPoint> MH("MidPoint");    
    RuleProxy<QuadratureRuleAdaptive<Simpson>> AD("Adaptive");
  }
}
