/*!
  \file bn.hpp
  \brief Header file for bn
  \author Daniele A. Di Pietro
  \author Tiziano Passerini
  \date 11-16-2005
*/

#ifndef BN_HPP
#define BN_HPP

#include "src/robust.hpp"
#include "src/bisection.hpp"
#include "src/newton.hpp"

using RootFinding::real;

real f(real x) {
    return exp(x) * (x - .6);
}

real df(real x) {
    return exp(x) * (x - .6) + exp(x);
}

#endif
