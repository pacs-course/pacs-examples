/*
 * main_concepts.cpp
 *
 *  Created on: Jan 13, 2023
 *      Author: forma
 */
#include "myConcepts.hpp"
#include "chrono.hpp"
#include <iostream>
#include <complex>
#include<memory>
#include <vector>
#include <concepts>
#include <cmath>
// to avoid useless warnings for this example
#pragma GCC diagnostic ignored "-Wunused-variable"
class Foo // a clonable class
{
public:
  [[nodiscard]] std::unique_ptr<Foo> clone()const {return std::make_unique<Foo>(*this);}
};

class Foo2 //a non clonable class
{
public:
  double data;
};



int main()
{
  using namespace apsc::concepts;
  std::complex a{2.,4.};
  auto x = complex_norm(a);
  /*
   This fails to compile
  double y{2.3};
  auto v = complex_norm(y);
  */
Foo foo;
auto p=apsc::concepts::cloneMe(foo);

/* This fails to compile
Foo2 foo2;
auto pp=apsc::concepts::cloneMe(foo2);
*/
using apsc::concepts::dot;
std::vector v1{1.,2.,3.,4.,5.};
std::vector v2{-1.,-2.,-3.,-4.,-5.};

std::cout<<"v11 *v2= "<<dot(v1,v2)<<std::endl;

/* This does not compile since string is not "dottable"
std::vector<std::string> s1{"A","pippo"};
std::vector<std::string> s2{"A","mickey"};
std::cout<<dot(s1,s2)<<std::endl;
*/

{
  std::array<double,3> a1{1.,2.,3.};
  std::cout<<"Small array. ";
  apsc::concepts::mul(a1);
}
{
  std::array<double,7> a1{1.,2.,3.,4.,5.,6.,7.};
  std::cout<<"Not small array. ";
  apsc::concepts::mul(a1);
}

// example of a constrained lambda
auto f=[](std::floating_point auto x){return std::pow(x,7.5);};
auto timeSpent=apsc::concepts::timeTaken(f,10.,Timings::Chrono{});
std::cout<<"Time spent is "<<timeSpent<<" milliseconds"<<std::endl;

//auto z = f("a");// error

// Concepts may be used as type traits:
std::cout<<"is Foo clonable? "<<std::boolalpha<<apsc::concepts::clonable<Foo><<std::endl;
std::cout<<"is Foo2 clonable? "<<std::boolalpha<<apsc::concepts::clonable<Foo2><<std::endl;

}


