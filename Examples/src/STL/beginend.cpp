#include <iostream>
#include <iterator>
using namespace std;
//! An example of use of begin() end() as free functions.
/*!
  C++11 has implemented begin() and end() also as free functions. This helps
  to use non-std containers with std algorithms. You need to define an iterator
  to your class and specialize the template function 
  \code{.cpp} template< class C >auto begin( C& c );\endcode
  
  Here we do not explain this technical details, found on any good
  reference book. We want just to show that we can use std::begin()
  and std::end() on (fixed-size) C-style arrays.

  We also show a nice use of ostream_iterators

 */
int main(){
  ostream_iterator<int> out_it (cout,", ");
  int vals[]={1,2,3,4,5,6};
  copy(begin(vals),end(vals),out_it);
  cout<<endl;

     int* v2=new int[2];
     v2[0]=10;
     v2[1]=9;
  /* This does not work: begin() and end() have bee defined only
     for fixed size C-style arrays not for pointers to dynamic arrays
     copy(begin(v2),end(v2),out_it);
     cout<<endl;
  */
  // This however works
     copy(v2,v2+2,out_it);
     cout<<endl;
  
}
