#include "mytemp.hpp"
//! An example of explicit template instantiation
/*!
   If you compile the program and analyze the symbols
   contained in the object file with nm --demangle main.o
   you will note that the code for the templates that
   are declared extern in mytemp.hpp has NOT been generated
   (the symbols are undefined).
*/
int
main()
{
  Myclass<double> a(5.0);
  Myclass<int>    b(5);
  Myclass<char>   c('A');
  double          d = func(5.0); // call on a double
  double          e = func(5);   // call on a int
}
