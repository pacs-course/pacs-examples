#include <iostream>
#include "concrete2.h"

double
concrete2::eval (double x) {return (3.0 * x);};


concrete2::~concrete2 ()
{ std::cout << "~concrete2 ()" << std::endl; };

