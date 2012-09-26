/*!
  \file bn.hpp
  \brief Header file for bn
  \author Daniele A. Di Pietro
  \date 8-26-2005
*/

#ifndef BN_HPP
#define BN_HPP

#include "src/bisection.hpp"
#include "src/newton.hpp"
#include "src/robust.hpp"

using RootFinding::real;

real f(real x) {
    return pow(x, 2) - .5;
}

real df(real x) {
    return 2 * x;
}

#endif
