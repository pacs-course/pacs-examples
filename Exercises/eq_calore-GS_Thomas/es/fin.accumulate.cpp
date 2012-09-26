/*!
  \file fin.accumulate.cpp
  \author Luca Formaggia
  \author Daniele A. Di Pietro
  \author Tiziano Passerini
  \date 22-10-2006
*/

#include <iostream>    // Input-output
#include <fstream>     // Input-output from a file
#include <cmath>       // Mathematical functions
#include <vector>      // Standard vector
#include <numeric>     // STL numeric algorithms

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

int main( )
{
  // Get the number of elements
  int M;
  do {
    cout << "Number of elements (1 < N < " << MMAX << ")"
	 <<endl;
    cin  >> M;
  } while (M <= 1 || M >= MMAX);
  
  // Grid stepsize  
  double h = L / M;
  
  // Vector of unknowns
  vector<double> uh(M + 1);
  
  // Initialize method with a linear profile
  uh[0] = (T0 - Tinf);
  for(int m = 1; m <= M; ++m)
    uh[m] = (1. - m * h) * (T0 - Tinf);
  
  // Gauss-Seidel iterations
  // The current iteration
  int iter = 0;
  // The new estimate for the m-th entry
  double uhm;
  // The difference between subsequent iterates
  vector<double> epsilon(M+1);
  // The Euclidean norm of the difference between 
  // subsequent iterates
  double error;
  
  do {
    epsilon[0] = 0.;
    
    for(int m = 1; m < M; ++m) {
      // Notice that uh[m-1] has already been updated
      uhm  = (uh[m-1] + uh[m+1]) / (2. + pow(h, 2) * a);
      epsilon[m] = pow(uhm - uh[m], 2);
      uh[m] = uhm;
    }
    
    // Last line
    uhm = 2 * uh[M-1] / (2 + pow(h, 2) * a); 
    epsilon[M] = pow(uhm - uh[M], 2);
    uh[M]=  uhm; 
    
    // Square modulus of the increment between two subsequent
    // iterates obtained by sum of epsilon elements, each 
    // of them being the square of the increment evaluated
    // on a node
    error = accumulate( epsilon.begin(), epsilon.end(), 0. );
    
    iter++;
  } while( (sqrt(error) > toler) && (iter < itermax) );
  
  // Convergence check
  if(iter < itermax)
    cout << "Convergence reached in " << iter << " iterations"
	 << endl;
  else
    cout << "Gauss-Seidel method failed to converge: "
	 << sqrt(error) << " > " << toler
	 << endl;
  
  // Interpolate analytical solution and compute Linf(0, L) error
  // The exact solution at interpolation nodes
  vector<double> uha(M + 1);
  double e = -1.;
  for(int m = 0; m <= M; ++m) {
    uha[m] = Tinf + (T0 - Tinf) * 
      cosh(sqrt(a) * (L - m * h)) / cosh(sqrt(a) * L);
    e = max(abs(uha[m] - (Tinf + uh[m])), e);
  }
  
  // Post-processing
  cout << "|| e ||_inf = " << e << endl;
  ofstream f("fin.xy");
  for(int m = 0; m <= M; ++m)
    f << m * h
      << "\t" << Tinf + uh[m]
      << "\t" << uha[m] <<endl;
  cout << "Output written to fin.xy" << endl;
  f.close();
  
  return 0;
}
