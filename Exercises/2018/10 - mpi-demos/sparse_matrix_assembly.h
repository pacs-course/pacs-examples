#include <sparse_matrix_simplified.h>

constexpr int msize = 6;

void build_matrix (int rank, int size, int &is, int &ie, sparse_matrix &A);

struct
non_local_t
{
  std::vector<int> row_ptr, col_ind;
  std::vector<double> a;
  
  void
  csr (sparse_matrix &A, int is, int ie);
};
