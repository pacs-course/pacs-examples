#include "sparse_matrix.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

int
main(int argc, char **argv)
{
  const unsigned int n = 10;

  std::vector<double> x(n, 1.0);

  sparse_matrix A;
  A.resize(n);

  for (unsigned int ii = 0; ii < n; ++ii)
    {
      if (ii > 0)
        A[ii][ii - 1] = -1;

      if (ii < n - 1)
        A[ii][ii + 1] = -1;

      A[ii][ii] = 4;
    }

  std::cout << "A:" << std::endl << A << std::endl;

  p_sparse_matrix A_ptr;
  A.extract_block_pointer({0, 1, 2, 3, 4}, {0, 2, 4}, A_ptr);
  std::cout << "A_ptr:" << std::endl << A_ptr << std::endl;

  p_sparse_matrix A_ptr2;
  A.extract_block_pointer_keep_cols({0, 1, 2, 3, 4}, {0, 2, 4}, A_ptr2);

  std::cout << "A_ptr2:" << std::endl << A_ptr2 << std::endl;
}
