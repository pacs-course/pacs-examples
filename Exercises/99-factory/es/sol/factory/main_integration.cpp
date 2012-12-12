#include <iostream>
#include <cmath>
#include <dlfcn.h>
#include <memory>
#include "GetPot"
#include "numerical_integration.hpp"
#include "rulesFactory.hpp"
#include "udfFactory.hpp"

void printHelp(){
	using std::cout;
	using std::endl;
	cout<<"*** Line Options ***"<<endl;
	cout<<"[-h  --help] This help"<<endl;
	cout<<"[InputFile=string] Input file name (quadrature.getpot)"<<endl;
	cout<<"*** File Options ***"<<endl;
	cout<<"[library=string] Quadrature rule library (libquadrules.so)"<<endl;
	cout<<"[rule=string] Quadrature rule name (Simpson)"<<endl;
	cout<<"              If rule=? the list of registered rules is given"<<endl;
	cout<<"[a=float] Left integration point (0)"<<endl;
	cout<<"[b=float] Right integration point (pi)"<<endl;
	cout<<"[nint=int] Number of intervals (10)"<<endl;
	cout<<"[fun=string] Name of the function (fsincos)" <<endl;
	cout<<"             If fun=? the list of registered functions is given" <<endl;
}

const double pi=4.0*std::atan(1.0);
int main(int argc, char** argv){

	using namespace Geometry;
	using namespace std;
	using namespace NumericalIntegration;

	// Process options given to the program
	{
		GetPot key_input(argc,argv);
		if (key_input.search(2, "--help", "-h")){
			printHelp();
			exit(0);
		}
	}

	// Get the factory with the rules
	RuleFactory & rulesFactory( RuleFactory::Instance());

	// Get the input file
	GetPot  cl( "quadratura.getpot" );

	// Load library with the rules
	string quadlib = cl("library", "libquadrules.so");

	void * dylib = dlopen( quadlib.c_str(), RTLD_NOW );

	if (!dylib)
	  {
	    cout<< "cannot find library" << quadlib <<endl;
	    cout<< dlerror();
	    exit(1);
	  }

	// Get the factory with the function
	UDFFactory & udfFactory( UDFFactory::Instance() );

	// Load all other info
	double a = cl("a", 0.);
	double b = cl("b", 1*pi);
	int nint = cl("nint", 10);
	string rule = cl("rule","Simpson");
	string fun = cl("fun", "fsincos");

	// Extract the rule. I usa an auto_ptr to be sure to eventually delete the object
	auto_ptr<QuadratureRule> theRule = rulesFactory.get(rule);

	bool status;

	if (!(rule=="?"))
	  if (status = (!theRule.get()) ){
	    cout << "Rule " << rule << "does not exist" << endl;
	    rule="?";
	  }
	if (rule=="?")
	  {
	    vector<string> lista = rulesFactory.registered();

	    cout << " The following rules are registered in " << quadlib << endl;

	    for (vector<string>::iterator i = lista.begin(); i<lista.end();++i) 
	      cout << *i << endl;

	    if (status==false) 
	      exit(0);
	    else 
	      exit(2); // exit with error status
	  }

	// Select the function
	FunPoint f = udfFactory.get( fun );

	if (fun=="?")
	  {
	    vector<string> lista = udfFactory.registered();

	    cout << " The following functions are registered" << endl;

	    for (vector<string>::iterator i = lista.begin(); i<lista.end();++i) 
	      cout << *i << endl;

	    if (status==false) 
	      exit(0);
	    else 
	      exit(2); // exit with error status
	  }


	cout << "Integral between " << a << " and " << b << " with " << nint << " intervals" << endl;
	cout << "Using rule " << rule << endl;

	// Compute integral
	Domain1D domain(a,b);
	Mesh1D mesh(domain, nint);

	Quadrature s(*theRule,mesh);

	double approxs = s.apply(f);
	cout<<" Result= "<<approxs<<endl;

	// Close rule library
	dlclose(dylib);
}

