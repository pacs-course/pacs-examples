#ifndef HH_FUNCTIONS2_HPP
#define HH_FUNCTIONS2_HPP
#include <iostream>
#include <typeinfo>
// Try to comment some of the definitions to see what happens!
template <class T>
void
funct(T &x)
{
  std::cout << " Calling funct(T&) with argument type " << typeid(T).name()
            << "\n";
}

template <class T>
void
funct(T const &x)
{
  std::cout << " Calling funct(T const &) with argument type "
            << typeid(T).name() << "\n";
}

template <class T>
void
funct(T &&x)
{
  std::cout << " Calling funct(T&&) with argument type " << typeid(x).name()
            << "\n";
}

#endif
