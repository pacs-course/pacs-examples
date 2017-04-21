#include "test_solve.h"
#include <iostream>

int main ()
{

  matrix A (10);
  matrix L (10);
  matrix U (10);
  std::vector<int> P (10);
  
  for (unsigned int i = 0; i < 10; ++i)
    {
      for (unsigned int j = 0; j < 10; ++j)
        {
          if (j+1 == i)
            A(i, j) = -1.;
          else if (j == i+1)
            A(i, j) = -1.;
          else if (i == j)
            A(i, j) = 10.;
        }
    }

  A.lu (L, U, P);
    
  for (unsigned int i = 0; i < 10; ++i)
    {
      for (unsigned int j = 0; j < 10; ++j)
        std::cout << L(i,j) << " ";
      std::cout << std::endl;
    }
  std::cout << std::endl;
  
  for (unsigned int i = 0; i < 10; ++i)
    {
      for (unsigned int j = 0; j < 10; ++j)
        std::cout << U(i,j) << " ";
      std::cout << std::endl;
    }
  std::cout << std::endl;
  
  matrix tmp = L * U;

  for (unsigned int i = 0; i < 10; ++i)
    {
      for (unsigned int j = 0; j < 10; ++j)
        std::cout << tmp(i,j) << " ";
      std::cout << std::endl;
    }
  return 0;
}

