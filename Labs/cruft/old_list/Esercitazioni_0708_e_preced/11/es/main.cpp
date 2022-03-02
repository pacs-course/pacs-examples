#include "src/pacsfem.hpp"
#include "src/polynomial.hpp"
#include "src/gauss.hpp"
#include "src/eigenSolver.hpp"

int main() {
  using namespace Approximation;
  using namespace Integration;
  
  Vector x(3);
  Vector y1(3), y2(3), y3(3);

  x[0] = 0.; 
  x[1] = 1.; 
  x[2] = 0.5;

  y1[0] = 1.; 
  y1[1] = 0.;
  y1[2] = 0.;

  y2[0] = 0.;
  y2[1] = 1.;
  y2[2] = 0.;

  y3[0] = 0.;
  y3[1] = 0.;
  y3[2] = 1.;

  UBLAS::vector<Polynomial<Real>*> phi(3);
  for(int i = 0; i < 3; i++)
    phi(i) = new Polynomial<Real>(2);

  phi(0)->interpolate(x, y1);
  phi(1)->interpolate(x, y2);
  phi(2)->interpolate(x, y3);
  
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++)
      std::cout << phi(i)->coeff(j) << " " << std::flush;
    std::cout << std::endl;
  }

  UBLAS::matrix<Polynomial<Real>*> M(3, 3);
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      M(i, j) = new Polynomial<Real>( ( *phi(i) ) * ( *phi(j) ) );

  Gauss<3, Real> qr;

  Matrix Mx(3, 3);

  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      Mx(i, j) = qr.apply(x(0), x(1), *M(i, j));

  std::cout << Mx << std::endl;

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++)
      delete M(i, j);
    delete phi(i);
  }
  
  return 0;
}
