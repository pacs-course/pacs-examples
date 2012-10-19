// file fdm.h

#ifndef FDM_ELLP_H
#define FDM_ELLP_H

#include <math.h>              
#include <complex>            
#include "matvec.h"      // my matrix-vector library

template<class T, class R> class fdm {
private:
  T bya, byb, byc, byd;  // domain=[bya,byb]X[byc,byd]
  int nx, ny;            // number of grid points in x, y
  int npt;               // npt = nx*ny 

  R a11(T, T);           // coefficient a11
  R a12(T, T);           // coefficient a12
  R a22(T, T);           // coefficient a22
  R b1(T, T);            // coefficient b1
  R b2(T, T);            // coefficient b2
  R rcc(T, T);           // coefficient c(x,y)
  R scf(T, T);           // source function f
  R bdg(T, T);           // boundary function g
public: 
  fdm(T a,T b,T c,T d,int mx, int my);  // constructor
              // domain=[a,b]X[c,d], nx = mx, ny = my
  T hx, hy;   // grid sizes in x, y directions
  Vcr<R> v;   // solution vector
  T BandSolve();   // form band matrix, solve using 
    // gauss elimination with partial pivoting and 
    // return the residual in linear system solving
};
#endif
