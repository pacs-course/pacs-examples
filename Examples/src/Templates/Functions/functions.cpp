#include "functions.hpp"
void fun(long int  x)
{
  std::cout<<" Calling fun overloaded for long int "<<"\n";
}

// Full specialization
template <>
void fun<double>(double  x)
{
  std::cout<<" Calling fun specialised for double"<<"\n";
}
