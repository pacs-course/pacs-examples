#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP 1

#include "pacsfem.hpp"
#include "linearAlgebra.hpp"
#include "linearSolver.hpp"

namespace Approximation {
  /*!
    \class Polynomial
    \brief A class to handle polynomials in one variable
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-19-2005
  */
  template<typename T>
  class Polynomial {
  public:
    /** @name Constructors and destructors
     */
    //@{
    //! Constructor taking the polynomial degree
    Polynomial(int D = 0) : M_D(D), M_c(D+1) {}
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the \f$i\f$-th coefficient
    inline T& coeff(int i) { return M_c(i); }
    //! Return the \f$i\f$-th coefficient (read-only version)
    inline const T& coeff(int i) const { return M_c(i); }
    //! Return the degree
    inline int degree() const { return M_D; }
    //@}
    /** @name Members
     */
    //@{
    /*! Interpolate a set of points and values
      \param P the vector of points
      \param v the vector of values
    */
    void interpolate(const UBLAS::vector<T>& P, 
		     const UBLAS::vector<T>& v);
    //! Compute the value at point \f$x\f$ using Horner's method
    T operator()(T x);
    //@}
    /** @name Binary operations
     */
    //@{
    //! Sum a polynomial
    template<typename _T>
    friend Polynomial<_T> operator+(const Polynomial<_T>&, 
				    const Polynomial<_T>&);
    //! Subtract a polynomial
    template<typename _T>
    friend Polynomial<_T> operator-(const Polynomial<_T>&, 
				    const Polynomial<_T>&);
    //! Multiply by a polynomial
    template<typename _T>
    friend Polynomial<_T> operator*(const Polynomial<_T>&, 
				    const Polynomial<_T>&);
    //@}
  private:
    //! Degree
    int M_D;
    //! Coefficient vector
    UBLAS::vector<T> M_c;
  };

  // Implementations

  template<typename T>
  void Polynomial<T>::interpolate(const UBLAS::vector<T>& P,
				  const UBLAS::vector<T>& v) {
    int N = M_D + 1;
    // Check if the dimensions are correct
    assert( (int)P.size() == N && (int)v.size() == N );

    typedef typename LinearAlgebra::LinearSolver<T> ls_type;
    typename ls_type::matrix_type A( N, N );

    for(int i = 0; i < N; i++)
      for(int j = 0; j < N; j++)
	A(i, j) = pow( P(i), j );

    std::cout << A << std::endl;
    std::cout << v << std::endl;

    M_c = v;

    ls_type::solve(A, M_c);
  }

  template<typename T>
  T Polynomial<T>::operator()(T x) {
    T v = M_c(M_D);
    for(int i = M_D - 1; i >= 0; i--)
      v = v * x + M_c(i);
    return v;
  }

  template<typename _T>
  Polynomial<_T> operator+(const Polynomial<_T>& P1, 
			   const Polynomial<_T>& P2) {
    int Nmin = std::min( P1.M_D, P2.M_D ) + 1;
    int Nmax = std::max( P1.M_D, P2.M_D ) + 1;
    int Dmax = std::max( P1.M_D, P2.M_D );

    Polynomial<_T> P( Dmax );
    for(int i = 0; i < Nmin; i++)
      P.M_c(i) = P1.M_c(i) + P2.M_c(i);

    for(int i = Nmin; i < Nmax; i++)
      P.M_c(i) = (P1.M_D == Dmax) ? P1.M_c(i) : P2.M_c(i);

    return P;
  }

  template<typename _T>
  Polynomial<_T> operator-(const Polynomial<_T>& P1, 
			   const Polynomial<_T>& P2) {
    int Nmin = std::min( P1.M_D, P2.M_D ) + 1;
    int Nmax = std::max( P1.M_D, P2.M_D ) + 1;
    int Dmax = std::max( P1.M_D, P2.M_D );

    Polynomial<_T> P( Dmax );
    for(int i = 0; i < Nmin; i++)
      P.M_c(i) = P1.M_c(i) - P2.M_c(i);

    for(int i = Nmin; i < Nmax; i++)
      P.M_c(i) = (P1.M_D == Dmax) ? P1.M_c(i) : -P2.M_c(i);

    return P;
  }

  template<typename _T>
  Polynomial<_T> operator*(const Polynomial<_T>& P1, 
			   const Polynomial<_T>& P2) {
    int N1 = P1.M_D + 1;
    int N2 = P2.M_D + 1;
    Polynomial<_T> P( P1.M_D + P2.M_D );

    for(int i1 = 0; i1 < N1; i1++)
      for(int i2 = 0; i2 < N2; i2++)
	P.M_c(i1 + i2) += P1.M_c(i1) * P2.M_c(i2);

    return P;
  }
}

#endif
