#include "cxxversion.hpp"
#include <iostream>
#include <string>
int
main()
{
  std::string version;
  std::cout<<"version id is "<< __cplusplus<<"\n";
  if(Utilities::is_cxx11())
    version = "C++11";
  else if(Utilities::is_cxx14())
    version = "C++14";
  else if(Utilities::is_cxx17())
    version = "C++17";
  else if(Utilities::is_cxx98())
    version = "C++98";
  else if(Utilities::is_cxx20())
      version = "C++20";
  else
    version = "UNKNOWN";

  std::cout << "You are compiling with C++ version: " << version << std::endl;
}
