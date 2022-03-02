/*!
  \file eig.hpp
  \author Daniele A. Di Pietro
  \date 19-8-2005
*/

#include <math.h>
#include "src/vector.hpp"
#include "src/matrix.hpp"
#include "src/cyclicJacobi.hpp"
#include "src/power.hpp"

#define N 4

int main(){
    using namespace LinearAlgebra;
    using namespace LinearAlgebra::Eigenvalues;

    Hilbert m( N );

    CyclicJacobi cj( 1e-15 );
    PowerMethod pm( 1e-15, 100 );

    std::cout << "* CYCLIC JACOBI METHOD *" << std::endl;
    std::cout << cj.apply( m ) << std::endl;

    pm.apply( m, Vector( N, 1. ) );
    std::cout << pm << std::endl;

    return 0;
}
