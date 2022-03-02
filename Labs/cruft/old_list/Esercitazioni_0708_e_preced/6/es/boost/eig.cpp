/*!
  \file eig.hpp
  \author Daniele A. Di Pietro
  \date 19-8-2005
*/

#include <math.h>
#include "src/linearAlgebra.hpp"
#include "src/hilbert.hpp"
#include "src/cyclicJacobi.hpp"
#include "src/power.hpp"

#define N 4

int main(){
  using namespace LinearAlgebra;
  using namespace LinearAlgebra::Eigenvalues;

  LinearAlgebra::Matrix m( LinearAlgebra::Hilbert( N ) );

  CyclicJacobi cj( 1e-15 );
  PowerMethod pm( 1e-15, 100);

  std::cout << "* CYCLIC JACOBI METHOD *" << std::endl;
  std::cout << cj.apply( m ) << std::endl;

  UBLAS::vector<double> v0( N );
  for(int i = 0; i < N; i++) v0[i] = 1.;
  pm.apply( m, v0 );
  std::cout << pm << std::endl;

  return 0;
}
