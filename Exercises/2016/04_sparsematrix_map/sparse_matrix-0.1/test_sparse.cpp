#include <iostream>

#include "matrix.h"
#include "sparse.h"

int main ()
{

  sparse_matrix A (2, 2);

  A(0, 0) = 1.0;   A(0, 1) = 2.0;
  A(1, 0) = 1.0;   A(1, 1) = 4.0;
  
  sparse_matrix  B (2);

  B(0, 0) = 1.0;   B(0, 1) = 2.0;
  B(1, 0) = 1.0;   B(1, 1) = 4.0;

  auto C = B * A;

  for (auto ii = 0u; ii < C.get_rows (); ++ii)
    {
      for (auto jj = 0u; jj < C.get_cols (); ++jj)
        std::cout << C(ii, jj) << " \t";
      std::cout << std::endl;
    }

  matrix f (2, 1);
  f (0, 0) = 1.0;
  f (1, 0) = 1.0;

  matrix uh (2, 1);
  A.gauss_seidel (f, uh);

  for (auto ii = 0u; ii < C.get_rows (); ++ii)
    {
      std::cout << uh(ii, 0) << std::endl;
    }

}
