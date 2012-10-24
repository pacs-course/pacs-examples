/*!
  \file eig.cpp
*/

#include <iostream>
#include <math.h>
#include "src/power.hpp"

int main()
{
    using namespace LinearAlgebra;
    using namespace LinearAlgebra::Eigenvalues;
    using namespace Eigen;

    typedef PowerMethod::Matrix Matrix;
    typedef PowerMethod::Vector Vector;

    Matrix C (5, 5);

    // Fill the matrix
    C(1, 0) = 0.5;
    C(4, 0) = 0.5;
    C(2, 1) = 1./3.;
    C(3, 1) = 1./3.;
    C(4, 1) = 1./3.;
    C(4, 2) = 1.;
    C(0, 3) = 0.5;
    C(4, 3) = 0.5;
    C(3, 4) = 1.;

    Vector ev(5);

    ev << 1, 1, 1, 1, 1;

    std::cout << C << std::endl;
    std::cout << ev << std::endl;

    PowerMethod pm( 1e-6, 100 );

    std::cout << "Power method solution" << std::endl;
    pm.apply( C, ev );
    std::cout << pm << std::endl;
    std::cout << "Autovalore calcolato:" << std::endl << ev << std::endl;

    return 0;
}
