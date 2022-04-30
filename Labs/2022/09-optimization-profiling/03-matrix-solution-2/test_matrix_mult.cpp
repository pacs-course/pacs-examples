#include "matrix.hpp"
#include "test_matrix_mult.hpp"

int
main(int argc, char **argv)
{
  matrix A(matrix_size);
  matrix B(matrix_size);
  for (size_t i = 0; i < matrix_size; ++i)
    {
      A(i, i)               = 10.0;
      A(i, matrix_size - 1) = 30.0;
      B(i, 0)               = 1.0;
      B(i, i)               = 3.0;
    }

  std::cout << "matrix_size = " << matrix_size << std::endl;

  tic();
  matrix C = A * B;
  toc("multiply_time = ");

  tic();
  matrix D = A.transpose();
  toc("transpose_time = ");

  return 0;
}
