#ifndef HH_Parameters_HH
#define HH_Parameters_HH
#include <iosfwd>
struct parameters
{
  //! max number of iteration for Gauss-Siedel
  int   itermax=1000000;
  //! Tolerance for stopping criterion
  double  toler=1.e-8;
  //! Bar length
   double L=40.0;
  //! First longitudinal dimension
  double a1=4.0;
 //! Second longitudinal dimension
  double a2=50.;
  //! Dirichlet condition
  double To=46.;
  //! External temperature 
  double Te=20.;
  //! Conductivity
  double k=0.164;
  //! Convection coefficient
  double hc=200e-6;
  //! Number of elements
  int M=100;
};
//! Prints parameters
std::ostream & operator << (std::ostream &,const parameters &);
#endif
