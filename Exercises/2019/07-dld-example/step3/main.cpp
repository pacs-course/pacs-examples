#include <cstring>
#include <dlfcn.h>
#include <functional>
#include <iostream>
#include <string>

#include "abstract.h"
#include "GetPot"

class
factory
{
private :
  abstract *myclass;
  void     *dylib;
public :
  factory (const char *which_class)
  {

    dylib = dlopen (which_class, RTLD_NOW);
    if (! dylib)
      {
        char *err = dlerror ();
        char st[255];
        sprintf (st, "loading library failed %s", err);
        throw (std::runtime_error (st));
      }

    std::function<abstract* (void)> s = (abstract* (*) (void)) dlsym (dylib, "generator");
    char *err = dlerror ();
    if (err != NULL)
      {
        char st[255];
        sprintf (st, "loading symbol failed %s", err);
        throw (std::runtime_error (st));
      }
    else
      {
        myclass = s ();
      }
    
  };

  ~factory ()
  {
    delete myclass;
  };

  double eval (double x) {return myclass->eval (x); };
  
};

int main (int argc, char ** argv)
{
  GetPot g (argc, argv);
  std::string opt =  g ("lib", "./libconcrete1.so");
  factory f (opt.c_str ());

  std::cout << "f.eval (1.0) = " << f.eval (1.0) << std::endl;

}
