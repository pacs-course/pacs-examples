#ifndef LINEARSOLVER_HPP
#define LINEARSOLVER_HPP 1

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"

#include "linearAlgebra.hpp"

#define F77NAME(X) X ## _

namespace LinearAlgebra {
  extern "C" {
    /*!
      \fn dgesv
      \brief Direct solver for linear equations (double precision)
      \author Daniele A. Di Pietro
      \date 11-20-2005
    */
    int F77NAME(dgesv)(int& N, int& NRHS, double* A, int& LDA,
		       int* IPIV, double* B, int& LDB, int& INFO);

    /*!
      \fn sgesv
      \brief Direct solver for linear equations (single precision)
      \author Daniele A. Di Pietro
      \date 11-20-2005
    */
    int F77NAME(sgesv)(int& N, int& NRHS, float* A, int& LDA,
		       int* IPIV, float* B, int& LDB, int& INFO);
  }

  /*!
    \class LinearSolver
    \brief Linear solver for full systems
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-19-2005
  */

  template<typename T>
  class LinearSolver {
  public:
    /** @name Typedefs
     */
    //@{
    //! Matrix type
    typedef UBLAS::matrix<T, UBLAS::column_major> matrix_type;
    //! Vector type
    typedef UBLAS::vector<T> vector_type;
    //@}
    /** @name Methods
     */
    //@{
    //! Solve a linear system
    static void solve(matrix_type& M, vector_type& b);
    //@}
  };

  // Implementations

  void LinearSolver<double>::solve(matrix_type& A, vector_type& x) {
    assert( A.size1() == A.size2() && A.size2() == x.size() );
    int N = x.size();

    int __N = N;
    int __NRHS = 1;
    double* __A = &(*A.begin1());    
    int __LDA = N;
    UBLAS::vector<int> iPiv(N);
    int* __IPIV = &(*iPiv.begin());
    double* __B = &(*x.begin());
    int __LDB = N;
    int __INFO;

    F77NAME(dgesv)(__N, __NRHS, __A, __LDA, __IPIV, __B, __LDB, __INFO);
  }

  void LinearSolver<float>::solve(matrix_type& A, vector_type& x) {
    assert( A.size1() == A.size2() && A.size2() == x.size() );
    int N = x.size();

    int __N = N;
    int __NRHS = 1;
    float* __A = &(*A.begin1());    
    int __LDA = N;
    UBLAS::vector<int> iPiv(N);
    int* __IPIV = &(*iPiv.begin());
    float* __B = &(*x.begin());
    int __LDB = N;
    int __INFO;

    F77NAME(sgesv)(__N, __NRHS, __A, __LDA, __IPIV, __B, __LDB, __INFO);
  }
}

#endif
