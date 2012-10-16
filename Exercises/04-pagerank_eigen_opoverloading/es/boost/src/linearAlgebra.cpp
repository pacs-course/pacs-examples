#include "linearAlgebra.hpp"

namespace LinearAlgebra {
  double FrobeniusNorm(const Matrix& A) {
    double norm = 0.;
    for(int i = 0; i < (int)A.size1(); i++)
      for(int j = 0; j < (int)A.size2(); j++)
	norm += pow(A(i, j), 2);
    return sqrt(norm);
  }

  double PsiNorm(const Matrix& A) {
    double norm = 0.;
    for(int i = 0; i < (int)A.size1(); i++)
      for(int j = 0; j < (int)A.size2(); j++)
	if(i != j) norm += pow(A(i, j), 2);
    return sqrt(norm);
  }

  Vector diag(const Matrix& A) {
    int N = A.size1();
    assert(N == (int)A.size2());
    Vector b(N);
    for(int i = 0; i < N; i++)
      b(i) = A(i, i);
    return b;
  }
}
