#ifndef GAUSS_HPP
#define GAUSS_HPP 1

#include "pacsfem.hpp" //!< Common definitions
#include "eigenSolver.hpp" //!< Eigenvalue/eigenvector problem solver

namespace Integration {
  /*!
    \class Gauss
    \brief \f$N\f$-point Gauss quadrature rule
    \author Daniele A. Di Pietro
    \date 11-12-2005
  */

  template<int N, typename T = double>
  class Gauss {
  public:
    /** @name Constructors
     */
    //@{
    //! Default constructor
    Gauss();
    //@}
    /** @name Accessors
     */
    //@{
    //! Return a reference to the vector of weights
    inline const UBLAS::vector<T>& weights() { return M_w; }
    //! Return a reference to the vector of points
    inline const UBLAS::vector<T>& nodes() { return M_x; }
    //@}
    /** @name Methods
     */
    //@{
    //! Approximate the integral using \f$N\f$-point Gauss quadrature rule
    template<class F>
    T apply(T a, T b, F f);
    //@}
  private:
    //! Number of nodes
    int M_N;
    //! Weights
    UBLAS::vector<T> M_w;
    //! Nodes
    UBLAS::vector<T> M_x;
  };

  // Implementation

  template<int N, typename T>
  Gauss<N, T>::Gauss() : M_N(N), M_w(N), M_x(N) {
    typedef LinearAlgebra::EigenSolver<T> es_type;
    typename es_type::vector_type e( M_N - 1 );
    typename es_type::eigvec_type z( M_N, M_N );

    M_x *= 0.;
    for(int k = 0; k < M_N - 1; k++)
      e(k) = sqrt( 1. / ( 4. - 1. / pow(k + 1, 2) ) );

    es_type es(es_type::EIGENVECTORS);
    es.solve(M_x, e, z);

    for(int k = 0; k < M_N; k++)
      M_w(k) = pow(z(0, k), 2) * 2;

    // Scale to reference interval (0, 1)
    for(int k = 0; k < M_N; k++) {
      M_x(k) = ( M_x(k) + 1 ) / 2.;
      M_w(k) /= 2.;
    }
  }

  template<int N, typename T>
  template<class F>
  T Gauss<N, T>::apply(T a, T b, F f) {
    T l = b - a;
    T I = 0.;
    for(int k = 0; k < M_N; k++) {
      T x = a + M_x(k) * l;
      I += l * M_w(k) * f(x);
    }
    return I;
  }
}

#endif
