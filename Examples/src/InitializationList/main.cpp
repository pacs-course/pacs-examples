#include <utility>
#include <vector>
#include <complex>
#include <set>
#include <array>
#include <tuple>
#include <iostream>
#include <complex>
//! This file illustrate uniform parameter listi initialization.
/*!
  With the C++11 standard we can initialize all sort of objects using the same
  syntax! It is very practical and it eases writing generic programs!
 */
//! A simple struct.
struct m{
  int i;
  double j;
};
//! A simple class.
class MyClass{
public:
  // explicit MyClass(int a, double b):i{a},j{b}{}
  MyClass(int a, double b):i{a},j{b}{}
private:
  int i;
  double j;
};
//! Since C+=17!
//! A class that derives from m
struct Derived: m // for struct inheritance is public by default
{
  double z=10.;
};

// To avoid useless warnings for this example.
// Pragmas are a way to provide compiler directives from a source file
// Here compiling with -Wall would produce some warnings about unused variables
// With the pragma I avoid this nuisance. Here variables are unused since it is
// just an example.
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

int main(){
  
  double c[]={1,2,3};// this was already possible in c++98
  int j{3}; // J is initialized to 3
  std::vector<int> v={1,3,4,5,6}; // initialization of a vector<>
  std::pair<double,double> a {1.0,2.0};// or of a pair<>
  std::set<double> mySet{10.0,11.0,-2.3}; // also for a set<>!!
  m pippo{3,4.5}; // We can use  initialization lists also for
                  // simple aggregates
  std::complex<double> cNumber={1.0,3.0}; //valid for std::complex
  MyClass foo{1,3.0};// Also a class object! if the constructor taking
                      // (int,double) is provided
  //  MyClass foo={1,3.0};// Also this form is ok unless the constructor 
                          // of MyClass is explicit! 
  
  std::vector<MyClass> aV(10,{1,3.0});
  // 10 Myclass objects initialized
  //  with MyCLass{1,3.0} are now stored in the vector

  std::pair<double, int> aPair{3.0,1};
  // An array with 3 pairs
  // Note the two brackets... it is due to the way the std::array class
  // is defined.
  std::array<std::pair<int,double>,3> arrayofPair
  {{
    {1,4.0},
    {2,8.0},
    {3,10.0}
    }};
  // but if the array stores plain old data you can just to
  std::array<int,4> intArray1{1,2,3,4};
  // and also
  std::array<int,4> intArray2={-1,-2,-3,-4};
  // Now the new C++17 extensione
  // An aggregate which derives publicly from an aggregate is an aggregate
  // and can be initialised as
  Derived d={{3,4.0}}; 
  // Here I have initialized only the variable inherited from the base class
  // since the one added in Derived has a default value
  std::cout<<"d.z is equal to "<<d.z<<std::endl;
  // But I can also avoid the double brackets
  Derived d2={5,6.0};
  // or specify Derived::z directly
  Derived d3{{8,9.0},7.0};
  std::cout<<"d3.z is equal to "<<d3.z<<std::endl;
  // But, to simplify further C++17 allows me to do
  Derived d4{8,9.0,7.0};
  // Beware of the use of brackets to indicate the initialization of 
  // aggregates elements of a container (an std::complex is an aggregate).
  //This vector contains 3+4i and 5+6i 
  std::vector<std::complex<double>> vc1={{3.,4.},{5.,6.}};
  std::cout<<"vc1=";
  for (auto i:vc1)std::cout<<i;
  std::cout<<std::endl;
  //This vector contains 3+0i and 5+0i 
  std::vector<std::complex<double>> vc2={{3},{5.}};
  std::cout<<"vc2=";
  for (auto i:vc2)std::cout<<i;
  std::cout<<std::endl;
  // BUT ALSO this vector contains 3+0i and 5_0i
  std::vector<std::complex<double>> vc3={3,5.};
  std::cout<<"vc3=";
  for (auto i:vc3)std::cout<<i;
  std::cout<<std::endl;
  // While here I have only 3 + 5i
  std::vector<std::complex<double>> vc4={{3,5.}};
  std::cout<<"vc4=";
  for (auto i:vc4)std::cout<<i;
  std::cout<<std::endl;
  // My advice, use always the internal brackets
  // in the case of containers of aggregates and specify
  // all aggregate members. Otherwise the result is not
  // always what you think!
  
}
