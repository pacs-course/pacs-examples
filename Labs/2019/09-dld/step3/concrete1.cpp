#include <iostream>
#include "concrete1.h"

double
concrete1::eval (double x) {return (2.0 * x);};


concrete1::~concrete1 ()
{ std::cout << "~concrete1 ()" << std::endl; };

extern "C" { abstract* generator (); }

abstract* generator ()
{
  return new concrete1 ();
}
