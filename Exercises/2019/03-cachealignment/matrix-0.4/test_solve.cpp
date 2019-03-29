#include "test_solve.h"
#include "test_matrix_mult.h"
#include <iostream>

int main ()
{

  matrix A (n);
  matrix rhs (n, 1);

  for (unsigned int i = 0; i < n; ++i)
    {
      rhs (i, 0) = 1.;
      for (unsigned int j = 0; j < n; ++j)
        {
          if (j+1 == i)
            A(i, j) = -1.;
          else if (j == i+1)
            A(i, j) = -1.;
          else if (i == j)
            A(i, j) = 2.;
        }
    }

  matrix uh = rhs;
  matrix LU = A;

  tic ();
  LU.solve (uh);
  toc ("Solve ");

  tic ();
  matrix f = A * uh;
  toc ("Multiply ");

  /*
  for (unsigned int i = 0; i < n; ++i)
    {
      std::cout << "x (" << i+1 << ") = " << uh(i, 0) << ";"        
                << "    y (" << i+1 << ") = " << f(i, 0) << ";"
                << "    b (" << i+1 << ") = " << rhs(i, 0) << ";"
                << std::endl;
    }
  */
  return 0;
}

