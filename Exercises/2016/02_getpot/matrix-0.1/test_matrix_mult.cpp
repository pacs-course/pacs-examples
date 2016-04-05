#include "matrix.h"
#include "test_matrix_mult.h"
int main ()
{

  matrix A(msize);
  matrix B(msize);
  for (unsigned int i = 0; i < msize; ++i)
    {
      A(i,i) = 10.0;
      A(i,msize-1) = 30.0;
      B(i,0) = 1.0;
      B(i,i) = 3.0;
    }

  std::cout << "msize = " << msize << std::endl;
  
  tic ();
  matrix C = A * B;
  toc ("multiply_time = ");

  return 0;
}
