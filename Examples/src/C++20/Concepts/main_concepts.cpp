/*
 * main_concepts.cpp
 *
 *  Created on: Jan 13, 2022
 *      Author: forma
 */
#include "myConcepts.hpp"
#include <iostream>
#include <complex>
#include<memory>
#include <vector>
#include <concepts>
class Foo // a clonable class
{
public:
  std::unique_ptr<Foo> clone()const {return std::make_unique<Foo>(*this);}
};

class Foo2 //a non clonable class
{
public:
  double data;
};



int main()
{
  std::complex a{2.,4.};
  auto x = apsc::concepts::complex_norm(a);
  /*
   This fails to compile
  double y{2.3};
  auto v = apsc::concepts::complex_norm(y);
  */
Foo foo;
auto p=apsc::concepts::cloneMe(foo);

/* This fails to compile
Foo2 foo2;
auto pp=apsc::concepts::cloneMe(foo2);
*/

std::vector v1{1.,2.,3.,4.,5.};
std::vector v2{-1.,-2.,-3.,-4.,-5.};
std::cout<<apsc::concepts::dot(v1,v2)<<std::endl;

/* This does not compile since string is not "dottable"
std::vector<std::string> s1{"A","pippo"};
std::vector<std::string> s2{"A","mickey"};
std::cout<<apsc::concepts::dot(s1,s2)<<std::endl;
*/

{
  std::array<double,3> a1{1.,2.,3.};
  std::cout<<"Small array. ";
  auto res=apsc::concepts::mul(a1);
  if (res<=5)
    std::cout<<"Using fast method\n";
  else
    std::cout<<"Using standard algorithm\n";
}
{
  std::array<double,7> a1{1.,2.,3.,4.,5.,6.,7.};
  std::cout<<"Not small array. ";
  auto res=apsc::concepts::mul(a1);
  if (res<=5)
    std::cout<<"Using fast method\n";
  else
    std::cout<<"Using standard algorithm\n";
}

auto f=[](std::floating_point auto x){return 3*x;};

auto y = f(4.5);//ok
auto z = f("a");// error

}


