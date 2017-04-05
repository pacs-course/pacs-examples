/*!
  \file bn.hpp
  \brief Header file for bn
  \author Daniele A. Di Pietro
  \author Luca Formaggia
  \author Tiziano Passerini
  \date 10-25-2006
*/

#ifndef BN_HPP
#define BN_HPP

#include "zerofun.hpp"

//! Function f(x) from exercise 3
real f(real x) {
  return pow(x,2) - 0.5;
}

//! First derivative of function f(x) from exercise 3
real df(real x) {
  return 2. * x;
}

#endif
