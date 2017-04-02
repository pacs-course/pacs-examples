/*!
  \file rootfinding.hpp
  \brief A collection of typedefs for rootfinding
*/

#ifndef ROOTFINDING_HPP
#define ROOTFINDING_HPP 1

#include <iostream>
#include <cmath>

//! Type for real numbers
typedef double real;

//! Type for function pointers
typedef real (*fctptr)(real const x);

//! Type for convergence check
enum checkT {
  INCREMENT, //! Check the difference between subsequent iterates
  RESIDUAL,  //! Check the residual
  BOTH       //! Check both conditions
};

#endif
