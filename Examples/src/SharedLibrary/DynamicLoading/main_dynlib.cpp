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

  std::cout<<"cube (1) ore square (2)?"<<std::endl;
  int answer;
  std::cin>>answer;
  Fun fn;
  if (answer ==1)
    // Unfortunately this is necessary. C is less type safe than C++!
    fn = reinterpret_cast<Fun>(dlsym(lib_handle,"cube"));
  else
    fn = reinterpret_cast<Fun>(dlsym(lib_handle,"square"));

  char* error=dlerror();
   
  if(error != 0 ){
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
