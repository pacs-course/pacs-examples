/*!
  \file tridiag.hpp
  \brief A wrapper of lapack routines for tridiagonal linear systems
  \author L. Formaggia
  \author D. A. Di Pietro
  \date 10-14-2005
*/

#ifndef TRIDIAG_HPP
#define TRIDIAG_HPP

#include <iostream>
#include <cassert>

namespace lapack {
  /* 
     Lapack routines are in Fortran. It is a common convention that 
     the Fortran name of a routine foo(..) is changed to the symbol
     foo_ in the object file. This macro appends '_' to the name
     passed as an argument.
  */
#define F77NAME(X) X ## _

  /*
    Declaring that dgttrf and dgttrs are external routine to be
    accessed following the "C" convention.
  */
  extern "C" {
    int F77NAME(dgttrf)(int&, double*, double*, double*, double*, 
			int*, int&);
  
    int F77NAME(dgttrs)(char*, int&, int&,
			double*, double*, double*, double*, 
			int*, double*, int&, int&);
  }


  /*! 
    \class TriDiag
    \brief Solve a tridiagonal system.

    Given the three sequences a, b, c containing the sub-diagonal,
    diagonal and super-diagonal elements of A (double precision), 
    this class computes the solution of Ax = b. 
    - a and c have length n-1, b has length n;
    - the sequence may be either an array or 
    a vector<double>::iterator;
    - the Lapack functions dgttrf and dgttrs are used.
    \author L. Formaggia
    \author D. A. Di Pietro
    \date 10-14-2005
  */

  class TriDiag {
  public:
    /** @name Constructors
     */
    //@{
    /*! 
      Constructor accepting three iterators as input 
      containing the starting location of the sequences a,b and c. 
      If C++ arrays are used, it is sufficient to pass their name.
      The iterators a,b and c must satisfy the following requirements:
      * they meet the specifications of pointer-type algebra;
      * they point to an area large enough to hold the right 
      number of doubles.
    */
    template<typename Iterator>
    TriDiag(Iterator, Iterator, Iterator, const int);
    //@}
    /** @name Destructors
     */
    //@{
    //! Destroy what has been created
    ~TriDiag();
    //@}

    /** @name Members
     */
    //@{
    //! Solve the system and overwrite the solution on rhs
    template<typename Iterator>
    int solve(Iterator);
    /*! 
      Return the status:
      - 0  : no problems were encountered
      - k  : the element u_kk of the LU decomposition is zero. 
      Singular matrix
      - -k : an illegal value in the matrix   
    */
    int status() const;
    //@}
  private:
    //! The dimension of the linear system
    int M_n;
    //! The number of rhs to solve for
    int M_nrhs;
    //! The array of sub-diagonal elements
    double* M_dl;
    //! The array of diagonal elements
    double* M_d__;
    //! The array of super-diagonal elements
    double* M_du;
    //! The array storing the elements of the second super-diagonal of U
    double* M_du2;
    //! The pivot indices
    int* M_ipiv;
    //! Successful exit
    int M_info;
  };

  // Implementation
  template<typename Iterator>
  TriDiag::TriDiag(Iterator a, Iterator b, Iterator c, 
		   const int dimension)
    : M_n(dimension), M_nrhs(1) {
    // Dimension the help array to be used by  Lapack routines checking
    // for memory allocation problems
    assert(M_dl   = new double[M_n - 1]);
    assert(M_d__  = new double[M_n]);
    assert(M_du   = new double[M_n - 1]);
    assert(M_du2  = new double[M_n - 2]);
    assert(M_ipiv = new int[M_n]);

    // Copy vectors using pointer algebra
    for(double *p = M_dl ; p != M_dl + M_n - 1; ++p, ++a)*p = *a;
    for(double *p = M_d__; p != M_d__+ M_n ; ++p, ++b) *p = *b;
    for(double *p = M_du; p != M_du + M_n - 1; ++p, ++c) *p = *c;

    // Factorization
    F77NAME(dgttrf)(M_n, M_dl, M_d__, M_du, M_du2, M_ipiv, M_info);
  }

  template<typename Iterator>
  int TriDiag::solve(Iterator rhs) {
    // remember rhs start position
    Iterator rhs_begin = rhs;
    // support variable
    double* b;
    assert(b   = new double[M_n]);
    // Copy vectors using pointer algebra
    for(double *p = b; p != b + M_n; ++p, ++rhs) *p=*rhs;
    // Check if factorisation has failed
    char* type="N";
    if(M_info == 0) 
      F77NAME(dgttrs)(type, M_n, M_nrhs, M_dl, M_d__, 
		      M_du, M_du2, M_ipiv, b, M_n, M_info);
    // solution is stored in b, we need to export it
    for( int i = 0; i != M_n; ++i, ++rhs_begin) *rhs_begin=b[i];
    delete [] b;
    return M_info;
  }
}

#endif
