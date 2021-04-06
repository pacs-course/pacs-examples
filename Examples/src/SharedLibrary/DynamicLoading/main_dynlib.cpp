#include <iostream>
#include <cstdlib>
//#include "functions.hpp"
#include <dlfcn.h>

typedef double(*Fun)(double);

int main(){
  void * lib_handle;
  lib_handle=dlopen("./libfunctions.so",RTLD_LAZY);
  if (!lib_handle){
    std::cerr<<"Error in opening library "<<dlerror()<<std::endl;
    std::exit(1);
  }

  int answer;

  std::cout<<"cube (1), square (2) or strangefun(3)?"<<std::endl;
  std::cin>>answer;
  Fun fn;
  if (answer ==1)
    // Unfortunately this is necessary. C is less type safe than C++!
    fn = reinterpret_cast<Fun>(dlsym(lib_handle,"cube"));
  else if (answer==2)
    fn = reinterpret_cast<Fun>(dlsym(lib_handle,"square"));
  else
    fn = reinterpret_cast<Fun>(dlsym(lib_handle,"strangefun"));

  char* error=dlerror();
   
  if(error != nullptr ){
    std::cerr<<"Error "<< error<<std::endl;
    std::exit(2);
  }

  std::cout<<" Give me a value"<<std::endl;
  double value;
  std::cin>>value;
  double result=fn(value);
  std::cout<<"Result= "<<result<<std::endl;
  dlclose(lib_handle);
}
