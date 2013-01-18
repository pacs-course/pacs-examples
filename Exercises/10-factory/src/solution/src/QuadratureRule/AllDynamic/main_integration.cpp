#include <iostream>
#include <dlfcn.h>
#include <memory>
#include <vector>
#include "GetPot"
#include "numerical_integration.hpp"
#include "ruleProxy.hpp"
#include "functionProxy.hpp"

void printHelp();

template <typename FactoryType>
void checkObject( bool, const std::string& , const FactoryType&, const std::string& );


int main(int argc, char** argv){

  using namespace Geometry;
  using namespace std;
  using namespace NumericalIntegration;
  using QuadratureRuleFactory::RulesFactory;
  using FunctionsFactory::FunctionsFactory;

  // Process options given to the program
  {
    GetPot key_input(argc,argv);
    if (key_input.search(2, "--help", "-h")){
      printHelp();
      exit(0);
    }
  }

  // Get the input file
  GetPot   cl("quadratura.getpot");

  // Load library with the rules
  string quadlib=cl("library","libmyrules.so");
  void * dylib=dlopen(quadlib.c_str(),RTLD_NOW);
  if (!dylib){
    cout<< "cannot find library" << quadlib <<endl;
    cout<< dlerror();
    exit(1);
  }

  // Now get the library with the functions to be integrated
  int libNumber = cl.vector_variable_size("udflib");
  vector<string> userdeflibs{"./libmyFunctions.so","./libmyFunctor.so"};

  vector<void *> dyFunlib(libNumber);

  for ( int i = 0; i < libNumber; ++i )
  {
      dyFunlib[i] = dlopen ( userdeflibs[i].c_str(),RTLD_NOW);
      if (!dyFunlib[i])
      {
        cout<< "cannot find library" << userdeflibs[i] <<endl;
        cout<< dlerror();
        exit(1);
      }
  }

  // Get the factory with the functions
  FunctionsFactory & functionFactory( FunctionsFactory::Instance());

  // Handle the library and get the integrand function
  std::string fun_name=cl("integrand","myfun");

  // Extract the function. I usa an auto_ptr to be sure to eventually delete the object
  std::unique_ptr<FunPoint> f = functionFactory.create( fun_name );

  for ( auto i : userdeflibs )  checkObject( f == nullptr, fun_name, functionFactory, i );

  // Load all other info
  double a=cl("a", 0.);
  double b=cl("b", 1 );
  int nint=cl("nint", 10);
  string rule=cl("rule","Simpson");
  // Get the factory with the rules
  RulesFactory & rulesFactory( RulesFactory::Instance());
  // Extract the rule. I usa an auto_ptr to be sure to eventually delete the object
  QuadratureRuleHandler theRule=rulesFactory.create(rule);

  checkObject( theRule == nullptr, rule, rulesFactory, quadlib );

  //  get the rule
  cout<<"Integral between "<<a<<" and "<<b <<" with "<< nint <<" intervals"<<endl;
  cout<<"Using rule "<<rule<<" and integrand "<<fun_name<<endl;

  // Compute integral
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  Quadrature s(*theRule,mesh);
  double approxs=s.apply(*f);
  cout<<"Result= "<<approxs<<endl;

  // Close rule library
  dlclose(dylib);

  for ( auto & i :dyFunlib ) dlclose( i );

}

void printHelp()
{
    using std::cout;
    using std::endl;
    cout<<"*** PROGRAM FOR COMPOSITE INTEGRATION  ***"<<endl<<endl;
    cout<<"*** Line Options ***"<<endl;
    cout<<"[-h  --help] This help"<<endl;
    cout<<"[InputFile=string] Input file name (quadrature.getpot)"<<endl<<endl;
    cout<<"*** File Options ***"<<endl;
    cout<<"[library=string] Quadrature rule library (libmyrules.so)"<<endl;
    cout<<"[udflib=string]  Library holding integrands (libudf.so)"<<endl;
    cout<<"[integrand=string] Integrand function name (myfun)"<<endl;
    cout<<"[rule=string] Quadrature rule name (Simpson)"<<endl;
    cout<<"              If rule=? the list of registered rules is given"<<endl;
    cout<<"[a=float] Left integration point (0)"<<endl;
    cout<<"[b=float] Right integration point (1)"<<endl;
    cout<<"[nint=int] Number of intervals (10)"<<endl;
}

template <typename FactoryType>
void checkObject( bool notThere, const std::string& objname,
                  const FactoryType& factory, const std::string& libname)
{
    using std::cout;
    using std::endl;

    if (notThere || objname=="?")
    {
        if (notThere)
        {
            cout <<"Object "<< objname<< "does not exist"<<endl;
        }
        auto lista = factory.registered();
        cout<<" The following objects are registered in " << libname << endl;
        for ( const auto & i : lista )
        {
            cout << i << endl;
        }
        if (notThere) std::exit(2);
        else std::exit(0); // exit without error status
  }

}

