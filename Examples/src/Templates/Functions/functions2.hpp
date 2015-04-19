#ifndef HH_FUNCTIONS2_HPP
#define HH_FUNCTIONS2_HPP
#include <iostream>
#include <typeinfo>
/*template <class T>
void funct(T const & x)
{
  std::cout<<" Calling fun with argument type "<<typeid(x).name()<<"\n";
}
*/

template <class T>
void funct(T&& x)
{
  std::cout<<" Calling fun with rvalue ref to type "<<typeid(x).name()<<"\n";
}

#endif
