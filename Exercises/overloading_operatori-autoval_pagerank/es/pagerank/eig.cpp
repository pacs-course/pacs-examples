/*!
  \file eig.hpp
  \author Daniele A. Di Pietro
  \date 19-8-2005
*/

#include <math.h>
#include "src/vector.hpp"
#include "src/matrix.hpp"
#include "src/power.hpp"


int main(){
    using namespace LinearAlgebra;
    using namespace LinearAlgebra::Eigenvalues;

    Matrix C(5,5,0.);
    C[1][0]=0.5;  C[4][0]=0.5;
    C(2,1)=1./3.; C(3,1)=1./3.;  C(4,1)=1./3.;
    C[4][2]=1.;
    C(0,3)=0.5; C(4,3)=0.5;
    C(3,4)=1.;
    Vector ev(5, 1.);
    std::cout << C << std::endl;

    PowerMethod pm( 1e-6, 100 );

    std::cout << "Power method solution" << std::endl;
    pm.apply( C, ev );
    std::cout << pm << std::endl;
    std::cout << "Autovalore calcolato:" << ev << std::endl;

    return 0;
}
