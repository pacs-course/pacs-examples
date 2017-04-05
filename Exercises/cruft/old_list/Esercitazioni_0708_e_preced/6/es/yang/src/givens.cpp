#include "givens.hpp"
//#include <cassert>

namespace LinearAlgebra {
  namespace EigenValues {
    Givens::Givens(int l, int m, double theta) :
      M_l(l), 
      M_m(m), 
      M_theta(theta), 
      M_c(cos(M_theta)),
      M_s(sin(M_theta)) {
      // l and m cannot be the same
      assert( M_l != M_m );
      // Swap l and m so that m > l
      if( M_l > M_m ) std::swap(M_l, M_m);
    }

    Givens::Givens(int l, int m, double c, double s) :
      M_l(l),
      M_m(m),
      M_theta(asin(s)),
      M_c(c),
      M_s(s) {
      if( M_l == M_m )
	std::cerr << "[Givens::Givens] ERROR: "
		  << "l and m cannot be the same"
		  << std::endl;
      if( M_l > M_m ) std::swap(M_l, M_m);
    }

    void Givens::apply(Matrix& A) {
      // Range check
      assert(M_m < A.size1() || M_m < A.size2());

      // A = G' * A
      for(int j = 0; j < A.size2(); j++) {
	double p1 = A[M_l][j];
	double p2 = A[M_m][j];
	A[M_l][j] = M_c * p1 - M_s * p2;
	A[M_m][j] = M_s * p1 + M_c * p2;
      }

      // A = A * G
      for(int i = 0; i < A.size2(); i++) {
	double p1 = A[i][M_l];
	double p2 = A[i][M_m];
	A[i][M_l] = M_c * p1 - M_s * p2;
	A[i][M_m] = M_s * p1 + M_c * p2;
      }
    }
  } // namespace LinearAlgebra
} // namespace EigenValues
