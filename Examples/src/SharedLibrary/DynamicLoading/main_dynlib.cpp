#include <cstdlib>
#include <iostream>
//#include "functions.hpp"
#include <dlfcn.h>
#include <functional>
#include <string>
#include <map>
using Fun=std::function<double (double)>; // I want to use function wrappers not pointers


// A simple factory for my functions
Fun functionFactory(void * lib_handle,std::string const & name)
{
  // Internally to the factory I need a pointer to function taking a double
  using Fun_p = double (*)(double);
  // I need reinterpret_cast unfortunately
  auto fn = reinterpret_cast<Fun_p>(dlsym(lib_handle, name.c_str()));
  auto error = dlerror();
  if(error != nullptr)
    {
      std::cerr << "Error " << error << std::endl;
      std::exit(2); //exit with status=2 (I could have thrown an exception instead)
    }
  return Fun{*fn};// return the function wrapper
}

int
main()
{
  void *lib_handle;
  lib_handle = dlopen("./libfunctions.so", RTLD_LAZY);
  if(!lib_handle)
    {
      std::cerr << "Error in opening library " << dlerror() << std::endl;
      std::exit(1);
    }

  int answer;
  std::map<int,std::string> inttoname{ {1,"cube"},{2,"square"},{3,"strangefun"} };

  std::cout << "cube (1), square (2) or strangefun(3)?" << std::endl;
  std::cin >> answer;
  Fun fn=functionFactory(lib_handle, inttoname[answer]);

  std::cout << " Give me a value" << std::endl;
  double value;
  std::cin >> value;
  double result = fn(value);
  std::cout << "Result= " << result << std::endl;
  dlclose(lib_handle);
}
