#ifndef HAVE_GAUSS_SEIDEL_H
#define HAVE_GAUSS_SEIDEL_H

#include "config.h"

void
gauss_seidel
(const double A[][nnodes],
 const double f[],
 double uh[],
 const unsigned int maxit = 10000,
 const double tol = 1.0e-15);


#endif
