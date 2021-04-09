#include <cmath>
#include "udfHandler.hpp"
#include "muParserFunction.hpp"
namespace
{
  double fsincos(double const & x)
  {
    using namespace std;
    return sin(x)*cos(x);
  };
  double pippo(double const & x)
  {
    return log(1+fabs(x))*3*x*x-2*x;
    
  };
  double pluto(double const & x)
  {
    return x*x*std::sin(x);
  };
  
  double one(double const & x)
  {
    return 1;
  };

  // a square wave =1 in (0.5, 0.5). Zero otherwise.
  double irregular(double const & x)
  {
    return (x>=-0.5 && x<=0.5)?1.0:0.0;
  }

  class parsedFunction
  {
    apsc::MuParserInterface::muParserFunction theFunction{"parsedFunction.txt"};
  public:
    double operator()(const double & x)
    {
      return theFunction(x);
    }
  };

  // load function objects to factory
  __attribute__((constructor)) void
  loadFunctionItems()
  {
    using namespace apsc::NumericalIntegration;
    addIntegrandToFactory("fsincos",fsincos);
    addIntegrandToFactory("pippo",pippo);
    addIntegrandToFactory("pluto",pluto);
    addIntegrandToFactory("one",one);
    addIntegrandToFactory("irregular",irregular);
    addIntegrandToFactory("parsedFunction",parsedFunction());
 }

}


