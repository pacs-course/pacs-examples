#include <iostream>
#include <dlfcn.h>
#include <memory>
#include "GetPot"
#include "numerical_integration.hpp"
#include "udfHandler.hpp"
#include "ruleProxy.hpp"
#include <string>
#include <filesystem>
void printHelp(){
	using std::cout;
	using std::endl;
	cout<<"*** PROGRAM FOR COMPOSITE INTEGRATION  ***"<<endl<<endl;
	cout<<"*** Line Options ***"<<endl;
	cout<<"[-h  --help] This help"<<endl;
	cout<<"[-l --list] List available rules"<<endl;
	cout<<"[InputFile=string] Input file name (quadrature.getpot)"<<endl<<endl;
	cout<<"*** File Options ***"<<endl;
	cout<<"[library=string] Quadrature rule library (libmyrules.so)"<<endl;
	cout<<"[udflib=string]  Library holding integrands (libudf.so)"<<endl;
	cout<<"[integrand=string] Integrand function name (myfun)"<<endl;
	cout<<"[rule=string] Quadrature rule name (Simpson)"<<endl;
	cout<<"              If rule=? the list of registered rules is given"<<endl;
	cout<<"[a=double] Left integration point (0)"<<endl;
	cout<<"[b=double] Right integration point (1)"<<endl;
	cout<<"[nint=int] Number of intervals (10)"<<endl;
	cout<<"[maxIter=int] Max n. Iteration (for adaptive rules) (1000)"<<endl;
	cout<<"[targetError=double] Target error (for adaptive rules) (1e-5)"<<endl;
}

//! Helper function
void printList(apsc::QuadratureRuleFactory::RulesFactory const & rulesFactory)
{
  auto lista=rulesFactory.registered();
  std::cout<<" The following rules are registered "<<std::endl;
  for (auto i : lista) std::cout<<i<<std::endl;
}

int main(int argc, char** argv){
  
  using namespace Geometry;
  using namespace std;
  using namespace apsc::NumericalIntegration;
  using apsc::QuadratureRuleFactory::RulesFactory;
  // Process options given to the program
  GetPot key_input(argc,argv);
  if (key_input.search(2, "--help", "-h")){
    printHelp();
    exit(0);
  }


  // Get the input file
  std::string inputFile=key_input("InputFile","quadratura.getpot");
  std::filesystem::path filepath(inputFile);
  if (!std::filesystem::exists(filepath))
    {
      std::cerr<<"Input file "<<inputFile<<" does not exists\n";
      return 1;
    }
  GetPot   cl(inputFile.c_str());
  //get the factory: this is the correct way
  RulesFactory const & rulesFactory=apsc::QuadratureRuleFactory::MyFactory;
  // This is wrong. You should get the global variable
  //RulesFactory const & rulesFactory=RulesFactory::Instance();
 // Load library with the rules
  auto nlibs = cl.vector_variable_size("library");
  if (nlibs ==0)
    {
      cout<<"You need to specify at least one plugin library\n";
      exit(1);
    }
  else
    {
      cout<<"Reading "<<nlibs<<" plugin libraries\n";
    }
  for (unsigned int i=0; i<nlibs;++i)
    {
      string quadlib=cl("library",i,"NONE");
      if(quadlib == string("NONE"))
        {
          cout<<"Getpot file wrongly parsed,. Cannot read library\n";
          exit(1);
        }
      else
        {
          cout<<"Reading plugin library "<<quadlib<<std::endl;
        }
      void * dylib=dlopen(quadlib.c_str(),RTLD_NOW);
      if (dylib==nullptr)
        {
        cout<< "cannot find library" << quadlib <<endl;
        cout<< dlerror();
        exit(1);
      }
    }

  if (key_input.search(2, "--list", "-l"))
    {
      printList(rulesFactory);
      exit(0);
    }
  // Now get the library with the functions to be integrated
  std::string userdeflib=cl("udflib","libudf.so");
  // Handle the library and get the integrand function
  UdfHandler integrands(userdeflib);
  std::string fun_name=cl("integrand","myfun");
  FunPoint f=integrands.getFunction(fun_name);
  
  // Load all other info
  double a=cl("a", 0.);
  double b=cl("b", 1 );
  int nint=cl("nint", 10);
  string rule=cl("rule","Simpson");
  if(rule=="?")
    {
       printList(rulesFactory);
       exit(0);
     }
  // Extract the rule. 
  bool notThere(false);
  QuadratureRuleHandler theRule; // alias to a unique_prt<QuadratureRule>
  try
    {
      theRule=rulesFactory.create(rule);
      std::string ruleKind{theRule->name()};

      // Here we need to treat in a special way Adaptive and Montecarlo
      // Alternatively, we may enrich the public interface of QuadratureRuleBase implementing
      // virtual dummy methods (i.e. methods that do nothing) in the base class, overridden specifically
      // in theadaptive variants and in Montecarlo. If we do this way we will always call setTargetError
      // and setMaxIter. For the standard quadrature rule they do nothing.
      if (ruleKind=="Adaptive" || ruleKind=="Montecarlo")
        {
	  double targetError=cl("targetError", 1.e-5);
	  unsigned int maxIter=cl("maxIter", 1000);
	  std::cout<<"target error="<<targetError<<" Max iterations="<<maxIter<<endl;
          theRule->setTargetError(targetError);
          theRule->setMaxIter(maxIter);
        }
    }  catch (std::invalid_argument &)
    {
      notThere = true;
    }
  if (notThere)
    {
      cout <<"Rule "<< rule<< "does not exist"<<endl;
      cout <<"Registered Rules are "<<endl;
      printList(rulesFactory);
      std::exit(2); // exit with error status
    }
  //  get the rule
  cout<<"Integral between "<<a<<" and "<<b <<" with "<< nint <<" intervals"<<endl;
  cout<<"Using rule "<<rule<<" and integrand "<<fun_name<<endl;
  // Compute integral
  Domain1D domain(a,b);
  Mesh1D mesh(domain,nint);
  Quadrature s(*theRule,mesh);
  double approxs=s.apply(f);
  cout<<"Result= "<<approxs<<endl;
}

