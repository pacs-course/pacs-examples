/*
 * setbc.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: forma
 */
#include <cmath>
#include "setbc.hpp"
namespace {
//! x^2
double fun1(double const t, double const * coord) {
	return coord[0] * coord[0];
}
//! t*sin(y)
double fun2(double const t, double const * coord) {
	return t * sin(coord[1]);
}

 double constant(double const t, double const * coord) {
    return 20;
  }
}

//! create the bc
/*! The mechanism is similar to that of a registration in a factory. Yet we
  have here a simple struct. It may well be defined as a singleton, since
  we do not need having more than one object of this type. In a more
  advance implementation it may load the bc via a file and a dynamic
  library for the functions.*/
namespace FEM{
  //! An object holding boundary conditions
  BCContainer & boundaryConditions(MyBc.bConditions);
  BcGet::BcGet(){
    bConditions.addBC(BCBase(Dirichlet,std::string("FarField1"),fun1));
    bConditions.addBC(BCBase(Neumann,std::string("FarField2"),fun2));
    bConditions.addBC(BCBase(Dirichlet,std::string("Wall1"),constant));
    bConditions.addBC(BCBase(Dirichlet,std::string("Wall2")));
  }
  // I create an obecjt of type MyBc whose only role is to register
  // the boudary conditions.
  BcGet MyBc;
}




