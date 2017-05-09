#include "test_chol.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

int main ()
{

  matrix A (5);

  /* 
     Example problem:
     
     Input :
     A =

   2.11334654980499e+01  -4.60601995221033e+00   6.19120742087912e+00   1.85107541595537e+00   5.14181678556525e+00
  -4.60601995221033e+00   1.04567122374055e+01   2.40543545940404e+00   5.32363136837376e+00  -8.45676498207572e-01
   6.19120742087912e+00   2.40543545940404e+00   1.51334704458685e+01   1.83115466287798e+00  -1.07035424165348e+00
   1.85107541595537e+00   5.32363136837376e+00   1.83115466287798e+00   3.73272325542306e+00   8.02234990593840e-01
   5.14181678556525e+00  -8.45676498207572e-01  -1.07035424165348e+00   8.02234990593840e-01   2.02274853771505e+00

     Expected output :
     R =

   4.59711491025076e+00  -1.00193709361924e+00   1.34675933531133e+00   4.02660244978388e-01   1.11848776590289e+00
   0.00000000000000e+00   3.07454619380412e+00   1.22125457122655e+00   1.86273721158743e+00   8.94369008876790e-02
   0.00000000000000e+00   0.00000000000000e+00   3.43922186124689e+00  -2.86695117509189e-01  -7.80965408520155e-01
   0.00000000000000e+00   0.00000000000000e+00   0.00000000000000e+00   1.36396380820354e-01  -2.83233176250211e-01
   0.00000000000000e+00   0.00000000000000e+00   0.00000000000000e+00   0.00000000000000e+00   2.71305537293990e-01    

  */

  unsigned int i = 0;
  unsigned int j = 0;
  A(i, j++) = 2.11334654980499e+01; A(i, j++) =-4.60601995221033e+00; A(i, j++) = 6.19120742087912e+00; A(i, j++) = 1.85107541595537e+00; A(i++, j) = 5.14181678556525e+00; j = 0;
  A(i, j++) =-4.60601995221033e+00; A(i, j++) = 1.04567122374055e+01; A(i, j++) = 2.40543545940404e+00; A(i, j++) = 5.32363136837376e+00; A(i++, j) =-8.45676498207572e-01; j = 0;
  A(i, j++) = 6.19120742087912e+00; A(i, j++) = 2.40543545940404e+00; A(i, j++) = 1.51334704458685e+01; A(i, j++) = 1.83115466287798e+00; A(i++, j) =-1.07035424165348e+00; j = 0;
  A(i, j++) = 1.85107541595537e+00; A(i, j++) = 5.32363136837376e+00; A(i, j++) = 1.83115466287798e+00; A(i, j++) = 3.73272325542306e+00; A(i++, j) = 8.02234990593840e-01; j = 0;
  A(i, j++) = 5.14181678556525e+00; A(i, j++) =-8.45676498207572e-01; A(i, j++) =-1.07035424165348e+00; A(i, j++) = 8.02234990593840e-01; A(i++, j) = 2.02274853771505e+00; j = 0;

  std::cout << "first run: using matrix that is actually SPD" << std::endl;
  std::cout <<  "A = " << std::endl;
  for (unsigned int i = 0; i < 5; ++i)
    {
      for (unsigned int j = 0; j < 5; ++j)
        std::cout << std::setw(10) << A(i,j) << " ";
      std::cout << std::endl;
    }
  std::cout << std::endl;
  
  matrix U (5);
  matrix L (5);   
  std::vector<int> P (5);
  bool chol_ok = true;

  
  try
    {
      U = A.chol ();
    }
  catch (std::invalid_argument &e)
    {
      std::cout << "Cholesky factorization failed because : " << e.what () << std::endl;
      chol_ok = false;
    }

  if (chol_ok)
    {
      matrix AA = U.transpose () * U;
      
      std::cout <<  "R = " << std::endl;
      for (unsigned int i = 0; i < 5; ++i)
        {
          for (unsigned int j = 0; j < 5; ++j)
            std::cout << std::setw(8) << U(i,j) << " ";
          std::cout << std::endl;
        }
      std::cout << std::endl;
      
      std::cout <<  "R'*R = " << std::endl;
      for (unsigned int i = 0; i < 5; ++i)
        {
          for (unsigned int j = 0; j < 5; ++j)
            std::cout << std::setw(8) << AA(i,j) << " ";
          std::cout << std::endl;
        }      
      std::cout << std::endl;
    }

  U.reset ();
  A(4, 4) = -100000.0;

  std::cout << "second run: using matrix that is not SPD" << std::endl;
  std::cout <<  "A = " << std::endl;
  for (unsigned int i = 0; i < 5; ++i)
    {
      for (unsigned int j = 0; j < 5; ++j)
        std::cout << std::setw(10) << A(i,j) << " ";
      std::cout << std::endl;
    }
  std::cout << std::endl;
  
  try
    {
      U = A.chol ();
    }
  catch (std::invalid_argument &e)
    {
      std::cout << "Cholesky factorization failed because : " << e.what () << std::endl;
      A.lu (L, U, P);
      chol_ok = false;
    }

  if (! chol_ok)
    {
      matrix AA = L * U;
      
      std::cout <<  "L = " << std::endl;
      for (unsigned int i = 0; i < 5; ++i)
        {
          for (unsigned int j = 0; j < 5; ++j)
            std::cout << std::setw(16) << L(i,j) << " ";
          std::cout << std::endl;
        }
      std::cout << std::endl;

      std::cout <<  "U = " << std::endl;
      for (unsigned int i = 0; i < 5; ++i)
        {
          for (unsigned int j = 0; j < 5; ++j)
            std::cout << std::setw(16) << U(i,j) << " ";
          std::cout << std::endl;
        }
      std::cout << std::endl;

      std::cout <<  "L*U = " << std::endl;
      std::vector<unsigned int> idx = {0, 1, 2, 3, 4};

      for (unsigned int i = 0; i < 5; ++i)
        std::swap (idx[i], idx[P[i]-1]);
      
      for (unsigned int i = 0; i < 5; ++i)
        {
          for (unsigned int j = 0; j < 5; ++j)
            std::cout << std::setw(16) << AA(idx[i],j) << " ";
          std::cout << std::endl;
        }      
      std::cout << std::endl;
    }

  
  return 0;
}

