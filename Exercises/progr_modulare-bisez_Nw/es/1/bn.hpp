/*!
  \file bn.hpp
  \brief Header file for bn
*/

#ifndef BN_HPP
#define BN_HPP 1

#include "rootfinding.hpp"

//! Function f(x)
real f(real x)
{
  return pow(x,2) - 0.5;
}

//! First derivative of function f(x)
real df(real x)
{
  return 2. * x;
}

#endif
