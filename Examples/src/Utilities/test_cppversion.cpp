#include "cxxversion.hpp"
#include<string>
#include<iostream>
int main()
{
  std::string version;
  if (Utilities::is_cxx11()) version="C++11";
  else if (Utilities::is_cxx14()) version="C++14";
  else if (Utilities::is_cxx98()) version="C++98";
  else version="UNKNOWN";

  std::cout<<"You are compiling with C++ version: "<<version<<std::endl;
}
