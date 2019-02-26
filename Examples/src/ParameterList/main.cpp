#include <utility>
#include <vector>
#include <complex>
#include <set>
#include <array>
#include <tuple>
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
  // but if the array stores plain old data you con just to
  std::array<int,4> intArray1{1,2,3,4};
  // and also
  std::array<int,4> intArray2={-1,-2,-3,-4};
}
