#ifndef EIGENSOLVER_HPP
#define EIGENSOLVER_HPP 1

#include <string>
#include <cassert>
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"

#include "linearAlgebra.hpp"

#define F77NAME(X) X ## _

namespace LinearAlgebra {
  namespace UBLAS = boost::numeric::ublas;

  extern "C" {
    /*!
      \fn dsteqr
      \brief LAPACK eigensolver (double precision)
      \author Daniele A. Di Pietro
      \date 11-20-2005
    */
    int F77NAME(dsteqr)(const char*, int&, double*, double*, double*, 
			int&, double*, int&);

    /*!
      \fn ssteqr
      \brief LAPACK eigensolver (single precision)
      \author Daniele A. Di Pietro
      \date 11-20-2005
    */
    int F77NAME(ssteqr)(const char*, int&, float*, float*, float*, 
			int&, float*, int&);

  }

  template<typename T>
  class EigenSolver {
  public:
    /** @name Typedefs
     */
    //@{
    //! Type for the output flag
    typedef enum modes {EIGENVALUES, EIGENVECTORS};
    //! Type for the vectors defining the tridiagonal matrix
    typedef typename UBLAS::vector<T> vector_type;
    //! Type for the eigenvector matrix
    typedef typename UBLAS::matrix<T, UBLAS::column_major> eigvec_type;
    //@}
    /** @name Constructors
     */
    //@{
    EigenSolver(modes mode) : M_mode(mode) {}
    //@}
    /** @name Methods
     */
    //@{
    /*! 
      Solve the eigenproblem
      \param D vector storing the main diagonal
      \param E vector storing the first subdiagonal
      \param Z matrix storing the eigenvectors
    */
    void solve(vector_type& d, vector_type& e, eigvec_type& z);
    //@}
  private:    
    /*!
      The required output
      \param EIGENVALUES compute eigenvalues only
      \param EIGENVECTORS compute eigenvalues and eigenvectors
    */
    modes M_mode;
    //! The order of the matrix
    int M_N;
    //! The leading dimension of the matrix storing the eigenvectors
    int M_LDZ;
    //! Exit code
    int M_INFO;
  };

  // Implementations

  template<>
  void EigenSolver<double>::solve(vector_type& d, 
				  vector_type& e, 
				  eigvec_type& z) {
    assert( e.size() == d.size() - 1);
    M_N = d.size();
    double* D = &( *d.begin() );
    double* E = &( *e.begin() );
    int LDZ = M_N;
    double* Z = &( *z.begin1() );
    double WORK[4 * M_N];
    int INFO;
  
    F77NAME(dsteqr)(M_mode == EIGENVALUES ? "N" : "I", 
		    M_N, D, E, Z, LDZ, WORK, INFO);
  }

  template<>
  void EigenSolver<float>::solve(vector_type& d, 
				 vector_type& e, 
				 eigvec_type& z) {
    assert( e.size() == d.size() - 1);
    M_N = d.size();
    float* D = &( *d.begin() );
    float* E = &( *e.begin() );
    int LDZ = M_N;
    float* Z = &( *z.begin1() );
    float WORK[4 * M_N];
    int INFO;
  
    F77NAME(ssteqr)(M_mode == EIGENVALUES ? "N" : "I",
		    M_N, D, E, Z, LDZ, WORK, INFO);
  }
}

#endif
