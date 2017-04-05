/*!
  \file fin-thomas.cpp
  \author Luca Formaggia
  \author Daniele A. Di Pietro
  \author Paolo Ferrandi
  \date 22-10-2008
*/

/* **** Thomas in versione efficiente ha qualche stupido errore. Lo rivedo asap ****/

#include <iostream>    // Input-output
#include <fstream>     // Input-output from a file
#include <cmath>       // Mathematical functions
#include <vector>      // Standard vector

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

//!  Solve with Thomas algorithm, efficient version
//
// *** da controllare! ***
//
bool Thomas_solver(const std::vector<double>& a, const std::vector<double>& c,
                   const std::vector<double>& e, const std::vector<double>& b,
                   std::vector<double>& uh)
{
    int n=e.size(); // = b.size()-1
    double alpha;   // no alpha vector anymore!
    vector<double> delta(n);

    if (a[0]==0.) return false;
    uh[0]=b[0]/(alpha=a[0]);
    for (int i=1; i<n; ++i)
    {
        delta[i]=c[i-1]/alpha;
        alpha=a[i]-delta[i]*e[i-1];
        if (alpha==0) return false;
        // Forward substitution
        uh[i]=(b[i]-e[i-1]*uh[i-1])/alpha;
    }
    // Backwards substitutions
    for (int i=n-2; i>=0; --i)
        uh[i] -= delta[i+1]*uh[i+1];

    return true;
}

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

    // Diagonal vectors:
    vector<double> d1(M+1,2+pow(h,2)*a);
    vector<double> d2(M,-1);
    vector<double> d3(M,-1);
    vector<double> b(M+1,0);
    d1[0]=1; d1[M]=2+pow(h,2)*a; d2[0]=0; b[0]=T0-Tinf;
    d3[M-1]=-2;
    if(Thomas_solver(d1, d2, d3, b, uh)==false)
    {
        cout << "Error: pivotal element is zero" << endl;
        return 1;
    }

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
