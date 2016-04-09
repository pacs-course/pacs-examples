#include "test_solve.h"
#include <iostream>

int main ()
{

  matrix A (10);
  matrix rhs (10, 1);

  for (unsigned int i = 0; i < 10; ++i)
    {
      rhs (i, 0) = 1.;
      for (unsigned int j = 0; j < 10; ++j)
        {
          if (j+1 == i)
            A(i, j) = -1.;
          else if (j == i+1)
            A(i, j) = -1.;
          else if (i == j)
            A(i, j) = 2.;
        }
    }

  matrix uh (rhs);
  A.solve (uh);
  
  matrix f = A * uh;
  
  for (unsigned int i = 0; i < 10; ++i)
    {
      std::cout << "x (" << i+1 << ") = " << uh(i, 0) << ";"        
                << "    y (" << i+1 << ") = " << f(i, 0) << ";"
                << "    b (" << i+1 << ") = " << rhs(i, 0) << ";"
                << std::endl;
    }
  return 0;
}

