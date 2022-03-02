/*!
  \file rootfinding.hpp
  \brief A collection of typedefs for rootfinding
  \author Daniele A. Di Pietro
  \author Luca Formaggia
  \author Tiziano Passerini
  \date 10-25-2006
*/

#ifndef ROOTFINDING_HPP
#define ROOTFINDING_HPP 1

#include <iostream>
#include <algorithm>
#include <cmath>

//! Type for real numbers
typedef double real;
//! Type for function pointers
typedef real (*fctptr)(real const x);
//! Type for convergence check
enum checkT {
  INCREMENT, //! Check the difference between subsequent iterates
  RESIDUAL   //! Check the residual
};
#endif
