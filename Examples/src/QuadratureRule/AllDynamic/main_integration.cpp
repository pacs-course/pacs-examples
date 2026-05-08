#include "LoadLibraries.hpp"
#include "QuadParameters.hpp"
#include "numerical_integration.hpp"
#include "ruleProxy.hpp"
#include "udfHandler.hpp"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
void
printHelp()
{
  using std::cout;
  using std::endl;
  cout << "*** PROGRAM FOR COMPOSITE INTEGRATION  ***" << endl << endl;
  cout << "*** Line Options ***" << endl;
  cout << "[-h  --help] This help" << endl;
  cout << "[-l --list] List available rules" << endl;
  cout << "[-i --integrands] List available integrands" << endl;

  cout << "[InputFile=string] Input file name (quadratura.getpot)" << endl
       << endl;
  cout << "*** File Options ***" << endl;
  cout << "[library=string] Quadrature rule library (libmyrules.so)" << endl;
  cout << "[udflib=string]  Library holding integrands (libudf.so)" << endl;
  cout << "[integrand=string] Integrand function name (myfun)" << endl;
  cout << "[rule=string] Quadrature rule name (Simpson)" << endl;
  cout << "              If rule=? the list of registered rules is given"
       << endl;
  cout << "[a=double] Left integration point (0)" << endl;
  cout << "[b=double] Right integration point (1)" << endl;
  cout << "[nint=int] Number of intervals (10)" << endl;
  cout << "[maxIter=int] Max n. Iteration (for adaptive rules) (1000)" << endl;
  cout << "[targetError=double] Target error (for adaptive rules) (1e-5)"
       << endl;
}

//! Helper function
template <class Factory>
void
printList(Factory const &rulesFactory)
{
  auto lista = rulesFactory.registered();
  std::cout << " The following rules are registered " << std::endl;
  for(auto i : lista)
    std::cout << i << std::endl;
}
/*!
Clears factories.

@details This is useful to ensure that you clear the factories before loading
new libraries. Otherwise, if you load a library with the same rule names as a
previously loaded library, you will get an error because the factory does not
allow duplicate entries. By clearing the factories, before closing the
libraries.
*/
void
clearFactories()
{
  apsc::QuadratureRuleFactory::MyFactory.clear();
  apsc::NumericalIntegration::myIntegrands.clear();
}

int
main(int argc, char **argv)
{
  using namespace Geometry;
  using namespace std;
  using namespace apsc::NumericalIntegration;
  using apsc::QuadratureRuleFactory::RulesFactory;
  // Process options given to the program
  GetPot key_input(argc, argv);
  if(key_input.search(2, "--help", "-h"))
    {
      printHelp();
      return 0;
    }
  // Get the input file
  std::string           inputFile = key_input("InputFile", "quadratura.getpot");
  std::filesystem::path filepath(inputFile);
  if(!std::filesystem::exists(filepath))
    {
      std::cerr << "Input file " << inputFile << " does not exists\n";
      return 1;
    }
  else
    {
      std::cout << "Reading input file " << inputFile << std::endl;
    }
  bool jsonFile = (inputFile.find(".json") != std::string::npos);

  // get the parameters
  QuadParameters parameters;
  if(jsonFile)
    parameters = readQuadParameters_json(inputFile);
  else
    parameters = readQuadParameters_GP(inputFile);

  // get the factory: this is the correct way to get a handle to the
  // factory fo rthe rules
  RulesFactory const &rulesFactory = apsc::QuadratureRuleFactory::MyFactory;
  // Now get the library with the functions to be integrated
  // Get a handle to the factory
  auto const &myIntegrands = apsc::NumericalIntegration::myIntegrands;
  // This is wrong. You should get the global variable
  // RulesFactory const & rulesFactory=RulesFactory::Instance();
  // Load library with the rules
  // @todo this part should be enucleated in a class!

  // Check if the quad libraries exists and open them
  auto nlibs = parameters.library.size();
  if(nlibs == 0)
    {
      cerr << "You need to specify at least one plugin library\n";
      return 1;
    }
  apsc::LoadLibraries QuadRuleLibraries;
  auto                good = QuadRuleLibraries.loadLibs(parameters.library);
  if(!good)
    {
      cerr << "cannot find one of the libraries" << endl;
      cerr << dlerror();
      clearFactories();
      return 1;
    }

  if(key_input.search(2, "--list", "-l"))
    {
      printList(rulesFactory);
      clearFactories();
      return 0;
    }
  //@ to do enucleate this part: useless code repetition!
  nlibs = parameters.udflib.size();
  if(nlibs == 0)
    {
      cerr << "You need to specify at least one myIntegrands library\n";
      clearFactories();
      return 1;
    }
  else
    {
      cout << "Reading " << nlibs << " integrand libraries\n";
    }
  apsc::LoadLibraries IntegrandLibraries;
  auto                good2 = IntegrandLibraries.loadLibs(parameters.udflib);
  if(!good2)
    {
      cerr << "cannot find one of the function libraries" << endl;
      cerr << dlerror();
      clearFactories();
      return 1;
    }
  if(key_input.search(2, "--integrands", "-i"))
    {
      printList(myIntegrands);
      clearFactories();
      return 0;
    }
  // get the integrand
  std::string fun_name = parameters.integrand;
  if(fun_name == "NULL")
    {
      std::cerr << "Getpot file must contain integrand=integrandName\n";
      std::cerr << " Valid integrands in the FunctionFactory:\n";
      printList(myIntegrands);
      clearFactories();
      return 2;
    }
  else if(fun_name == "?")
    {
      std::cerr << " Valid integrands in the FunctionFactory:\n";
      printList(myIntegrands);
      clearFactories();
      return 0;
    }

  FunPoint f;
  try
    {
      f = myIntegrands.get(fun_name);
    }
  catch(std::invalid_argument &)
    {
      std::cerr << "Integrand not present. Factory provides\n";
      printList(myIntegrands);
    }

  // Load all other info
  double a = parameters.a;
  double b = parameters.b;
  int    nint = parameters.nint;
  // the rule
  string rule = parameters.rule;
  if(rule == "?")
    {
      printList(rulesFactory);
      return 0;
    }
  // Extract the rule.
  bool                  notThere(false);
  QuadratureRuleHandler theRule; // alias to a PointerWrapper<QuadratureRule>
  try
    {
      theRule = rulesFactory.create(rule);
      double       targetError = parameters.targetError;
      unsigned int maxIter = parameters.maxIter;
      theRule->setTargetError(targetError);
      theRule->setMaxIter(maxIter);

      std::string ruleKind{theRule->name()};
      if(ruleKind == "Adaptive" || ruleKind == "Montecarlo")
        {
          std::cout << "target error=" << targetError
                    << " Max iterations=" << maxIter << endl;
        }
    }
  catch(std::invalid_argument &)
    {
      notThere = true;
    }
  if(notThere)
    {
      cout << "Rule " << rule << "does not exist" << endl;
      cout << "Registered Rules are " << endl;
      printList(rulesFactory);
      clearFactories();
      return 2; // exit with error status
    }
  //  get the rule
  cout << "Integral between " << a << " and " << b << " with " << nint
       << " intervals" << endl;
  cout << "Using rule " << rule << " and integrand " << fun_name << endl;
  // Compute integral
  Domain1D            domain(a, b);
  Mesh1D              mesh(domain, nint);
  CompositeQuadrature s(*theRule, mesh);
  double              approxs = s.apply(f);
  cout << "Result= " << approxs << endl;
  clearFactories();
  return 0;
}
