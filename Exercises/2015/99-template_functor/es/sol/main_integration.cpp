#include <iostream>
#include <cmath>
#include "GetPot"
#include "./src/numerical_integration.hpp"
#include "./src/mesh.hpp"
const double pi=2*std::atan(1.0);
using namespace NumericalIntegration;
using namespace Geometry;

// Using functions
double fsincos(double const &);

double fsincos(double const & x)
{
  using namespace std;
  return sin(x)*cos(x);
};
double exact(double const& a, const double & b)
{
  using namespace std;
  return 0.5*(sin(b)*sin(b)-sin(a)*sin(a));
};

double xsinkx(const double& x, const double& k )
{
  return x*std::sin(k*x);
}

double primit( const double& x, const double& k)
{
  return (std::sin(k*x)-k*x*std::cos(k*x))/(k*k);
}

// Using functors
class functor
{
public:
  functor( const FunPoint& f, double k=1.0):M_k(k){}
  void setK(double const & k){M_k=k;}
  double operator() (double const & x) const
  {
    return M_f(x, M_k);
  }
  operator FunPoint ()
  {
    return M_f;
  } 
  
private:
  double M_k;
  FunPoint M_f;
};

int main(int argc, char* argv[])
{


  FunPoint f=fsincos;
  
  using namespace std;
  cout.precision(10);

  // Select the name for the data file, if specified with argc and argv
  string fileName("data");
  if ( argc == 2 )
    fileName = argv[1];

  // Read, using GetPot, the data from file
  GetPot cl( fileName.c_str() );

  // Define the master section
  string section = "integration/";

  // Create the domain of integration
  Domain1D domain ( cl, section + "domain/" );

  // Create the mesh of the domain
  Mesh1D mesh ( domain, cl, section + "mesh/" );

  cout << "Integrale tra " << domain.left() << " e " << domain.right()
       << " su " << mesh.numNodes() << " intervalli" << endl;

  Quadrature<Simpson>     s ( mesh );
  Quadrature<MidPoint>    m ( mesh );
  Quadrature<Trapezoidal> t ( mesh );
  Quadrature<Gauss4Pt>    g ( mesh );

  double approxs = s.apply(f);
  double approxm = m.apply(f);
  double approxt = t.apply(f);
  double approxg = g.apply(f);
  double exactVal= exact(domain.left(), domain.right());

  cout << "MidPoint= " << approxm 
       << " Simpson= " << approxs
       << " Trapezoidal= " << approxt
       << " Gauss= " << approxg
       << " Exact= " << exactVal 
       << endl;

  // With functors Integrand = xsin(kx)
  double k=2.0;
  functor integrand(xsinkx, k);
  funcotr primitive(primit, k);

  cout << "Integral of xsin(" << k << "x)" << endl;
  approxs = s.apply( integrand );
  approxm = m.apply( integrand );
  approxt = t.apply( integrand );
  approxg = g.apply( integrand );
  exactVal= primitive( domain.right() ) - primitive( domain.left() );
  
  cout << " MidPoint= " << approxm 
       << " Simpson= " << approxs
       << " Trapezoidal= " << approxt
       << " Gauss= " << approxg
       << " Exact= " << exactVal 
       << endl;

  return 0;  
}
  
