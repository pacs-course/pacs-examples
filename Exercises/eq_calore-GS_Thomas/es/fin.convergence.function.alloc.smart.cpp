/*!
  \file fin.convergence.function.alloc.smart.cpp
  \author Luca Formaggia
  \author Daniele A. Di Pietro
  \author Tiziano Passerini
  \date 20-10-2006
*/

#include <iostream>    // Input-output
#include <fstream>     // Input-output from a file
#include <cmath>       // Mathematical functions
#include <vector>      // Standard vector
#include <memory>      // STL auto pointer

// Avoid using scope operator to access namespace std
using namespace std;

/** @name Global variables
 */
//@{
//! The maximum number of elements
const int MMAX = 501;
//! The maximum number of iterations
const int itermax = 1000000;
//! The tolerance
const double toler = 1e-8;
//! Fin length
const double L = 1.e-1;
//! Fin width
const double w = 5.e-2;
//! Fin thickness
const double t = 5.e-3;
//! Base temperature
const double T0 = 393.;
//! Fluid asymptotic temperature
const double Tinf = 293.;
//! Fin thermal conductivity
const double k = 5;
//! The convection heat transfer coefficient
const double hP = 10;
//! The parameter a
const double a = 2. * hP * (w + t) / (k * w * t);
//@}

//! Solve problem with Gauss-Seidel method
/*!
  \param vec_ptr const reference to a smart pointer to
  the nodal values of the solution
  \param dx spatial discretization spacing
  \return true = convergence achieved;
  false = convergence failed
*/
bool solve_GS( const std::auto_ptr< std::vector<double> >& vec_ptr,
	       const double dx )
{
  // Gauss-Seidel iterations
  // The current iteration
  int iter = 0;
  // The new estimate for the m-th entry
  double vecm;
  // The Euclidean norm of the difference between
  // subsequent iterates
  double epsilon;

  do {
    epsilon = 0.;
    
    for(int m = 1; m < vec_ptr->size() - 1; ++m) {
      // Notice that (*vec_ptr)[m-1] has already been updated
      vecm  = ((*vec_ptr)[m-1] + (*vec_ptr)[m+1]) /
	(2. + pow(dx, 2) * a);
      epsilon += pow(vecm - (*vec_ptr)[m], 2);
      (*vec_ptr)[m] = vecm;
    }
    
    // Last line
    vecm = 2 * (*vec_ptr)[vec_ptr->size() - 2] /
      (2 + pow(dx, 2) * a); 
    epsilon += pow(vecm - (*vec_ptr)[vec_ptr->size() - 1], 2);
    (*vec_ptr)[vec_ptr->size() - 1]=  vecm; 
    
    iter++;     
  } while( (sqrt(epsilon) > toler) && (iter < itermax) );
  
  // Convergence check
  if(iter < itermax)
    {
      cout << "Convergence reached in " << iter
	   << " iterations" << endl;
      return true;
    }
  else
    cout << "Gauss-Seidel method failed to converge: "
	 << sqrt(epsilon) << " > " << toler
	 << endl;

  return false;
}

int main( )
{
  // This constructor creates and opens the file.
  // If existing, the file is emptied
  ofstream f("fin.convergence.xy");

  // Consider different values for M
  for( int M = 20; M < MMAX; M *= 2 )
    {
      // Grid stepsize  
      double h = L / M;
  
      // Allocate the vector of unknowns
      auto_ptr< vector<double> > uh_ptr( new vector<double>(M + 1) );
  
      // Initialize method with a linear profile
      (*uh_ptr)[0] = (T0 - Tinf);
      for(int m = 1; m <= M; ++m)
        (*uh_ptr)[m] = (1. - m * h) * (T0 - Tinf);

      // res == true : converged
      // res == false : not converged
      bool res = solve_GS( uh_ptr, h );

      // Interpolate analytical solution and compute Linf(0, L) error
      // The exact solution at interpolation nodes
      vector<double> uha(M + 1);
      double e = -1.;
      for(int m = 0; m <= M; ++m) {
        uha[m] = Tinf + (T0 - Tinf) * 
	  cosh(sqrt(a) * (L - m * h)) / cosh(sqrt(a) * L);
        e = max(abs(uha[m] - (Tinf + (*uh_ptr)[m])), e);
      }

      // Post-processing
      cout << "|| e ||_inf = " << e << endl;
      f << h
	<< "\t" << e << endl;
      cout << "Output written to fin.convergence.xy" << endl;

      // This is useless now
      // delete uh;
    }
  
  f.close();
  
  return 0;
}
