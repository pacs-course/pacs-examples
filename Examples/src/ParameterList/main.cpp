#include <utility>
#include <vector>
#include <complex>
#include <set>
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
// Here compiling with -Wall would produce some working about unused variables
// With the pragma I avoid this nuisance. Here variables are unused since it is
// just an example.
#pragma GCC diagnostic ignored "-Wunused-variable"

int main(){
  
  double c[]={1,2,3};// this was already possible in c++98
  /*
         **********  New c++11 stuff *************
  */

  int j{3}; // J is initialized to 3
  std::vector<int> v={1,3,4,5,6}; // initialization of a vector<>
  std::pair<double,double> a {1.0,2.0};// or of a pair<>
  std::set<double> mySet{10.0,11.0,-2.3}; // also for a set<>!!
  m pippo{3,4.5}; // We cannot use parameter lists also for
                  // structs
  std::complex<double> cNumber={1.0,3.0}; //valid for std::complex
  MyClass foo{1,3.0};// Also a class object! if the constructor taking
                      // (int,double) is provided
  //  MyClass foo={1,3.0};// Also this form is ok unless the constructor 
                          // of MyClass is explicit! 
  
  std::vector<MyClass> aV(10,{1,3.0});// 10 Myclass objects initialized
                                      //  with MyCLass{1,3.0}
}
