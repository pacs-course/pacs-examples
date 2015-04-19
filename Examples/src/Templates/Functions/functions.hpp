#ifndef HH_FUNCTIONS_HPP
#define HH_FUNCTIONS_HPP
#include <iostream>
#include <typeinfo>
template <class T>
void fun(T x);

// Overloading
template <class T>
void fun(T* x);

// Full specialization
template <>
void fun(double x);
// Overloading with simple function

void fun(long int  x);

template <class T>
void fun(T x)
{
  std::cout<<" Calling fun with argument type "<<typeid(T).name()<<"\n";
}


template <class T>
void fun(T* x)
{
  std::cout<<" Calling fun with argument pointer to type "<<typeid(T).name()<<"\n";

}




#endif
