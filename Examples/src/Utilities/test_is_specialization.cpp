/*
 * test_is_specialization.cpp
 *
 *  Created on: Apr 15, 2023
 *      Author: forma
 */
#include <iostream>
#include <vector>
#include "is_specialization.hpp"
template <class T>
class MyClass{};


template <class T>
class AnotherClass{};

template <class T>
void fun(const T& v)
{
  if constexpr(apsc::TypeTraits::is_specialization_v<T,MyClass>)
    {
      std::cout<<"is specialization of MyClass\n";
    }
  else
    std::cout<<"is not specialization of MyClass\n";
  (void)(v); //a trick so the compiler deas not complain
}

template<class T>
requires apsc::TypeTraits::Specialization_of<T,std::vector>
class Foo
{};

int main()
{
  std::cout<<"MyClass<double> ";
  fun(MyClass<double>{});
  std::cout<<"MyClass<int> ";
  fun(MyClass<int>{});
  std::cout<<"AnotherClass<int> ";
  fun(AnotherClass<int>{});
  Foo<std::vector<int>> foo;//ok
 // Foo<MyClass<int>> foo2;//fails
  (void)(foo);// to avoid useless warnings (a dirty trick).




}



