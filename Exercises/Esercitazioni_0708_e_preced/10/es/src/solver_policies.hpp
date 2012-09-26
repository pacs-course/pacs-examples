#ifndef _HPP_SOLVER_POLICIES_HPP
#define _HPP_SOLVER_POLICIES_HPP

#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

//! Using boost linear algebra classes
namespace UBLAS=boost::numeric::ublas;
using namespace UBLAS;

/*!
  \class IdentityPrecond
  \brief A trivial preconditioner for coniugate gradient algorithm

  The problem reads
  \f[
  P \mathbf{z} = \mathbf{r}
  \f]
  with \f$ \mathbf{r} \f$ the residual at n-th step.
  An object of type IdentityPrecond features an identity
  matrix: \f$P = I\f$.
*/
template<typename V>
class IdentityPrecond
{
public:
  /*! \name Methods
   */
  //@{
  //! Solver
  /*!
    \param rhs contains the right hand side for linear system
    \f[
    P \mathbf{z} = \mathbf{r}
    \f]
    On exit it contains the solution
  */
  V solve(V const & rhs)const { return rhs;}
};


/*!
  \class UblasTriPrecond
  \brief A triangular preconditioner for coniugate gradient algorithm

  The problem reads
  \f[
  P \mathbf{z} = \mathbf{r}
  \f]
  with \f$ \mathbf{r} \f$ the residual at n-th step.
  An object of type UblasTriPrecond features a lower
  or upper triangular matrix \f$P\f$.
*/
template<typename T,typename C=UBLAS::lower_tag>
class UblasTriPrecond
{
public:
  //! Matrix type
  typedef typename UBLAS::coordinate_matrix<T> M;
  //! Vector type
  typedef typename UBLAS::vector<T> V;

  /*! \name Constructors
   */
  //@{
  //! Copy constructor taking a matrix as a parameter
  explicit UblasTriPrecond(const M & m):matrix(m) {};
  //@}

  /*! \name Methods
   */
  //@{
  //! Solver
  /*!
    \param rhs contains the right hand side for linear system
    \f[
    P \mathbf{z} = \mathbf{r}
    \f]
    On exit it contains the solution
  */
  V solve(V const & rhs)const { return UBLAS::solve(matrix,rhs,C());}
private:
  M const matrix;
};

#endif
