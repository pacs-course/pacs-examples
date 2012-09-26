/*!
  \file bn.hpp
  \brief Header file for bn
*/

#ifndef BN_HPP
#define BN_HPP

#include "./src/rootfinding.hpp"

using RootFinding::real;

real f(real x) {
    return exp(x) * (x - .6);
}

real df(real x) {
    return exp(x) * (x - .6) + exp(x);
}

#endif
