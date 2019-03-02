#include "matrix.h"
#include <cassert>
#include <algorithm>
#include <ctime>
#include <Eigen/Dense>

matrix
matrix::transpose () const
{
  matrix retval (this->get_cols (), this->get_rows ());

  Eigen::Map<Eigen::MatrixXd>
    eigen_retval (retval.get_data (), get_rows (), get_cols ());

  Eigen::Map<const Eigen::MatrixXd>
    eigen_this (this->get_data (), this->get_rows (), this->get_cols ());

  eigen_retval = eigen_this.transpose ();
  return (retval);
}

#if defined (MAKE_TMP_TRANSP)
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  matrix tmp = A.transpose ();
  for (unsigned int i = 0; i < retval.get_rows (); ++i)
    for (unsigned int j = 0; j < retval.get_cols (); ++j)
      for (unsigned int k = 0; k < A.get_cols (); ++k)
        retval(i,j) += tmp(k,i) * B(k,j);
  return (retval);
}
#else
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());

  Eigen::Map<const Eigen::MatrixXd>
    eigen_A (A.get_data (), A.get_rows (), A.get_cols ());

  Eigen::Map<const Eigen::MatrixXd>
    eigen_B (B.get_data (), B.get_rows (), B.get_cols ());

  matrix retval (A.get_rows (), B.get_cols ());

  Eigen::Map<Eigen::MatrixXd>
    eigen_retval (retval.get_data (), A.get_rows (), B.get_cols ());

  eigen_retval = eigen_A * eigen_B;
    
  return (retval);
}
#endif

void
matrix::solve (matrix &rhs)
{

  Eigen::Map<const Eigen::VectorXd> f (rhs.get_data (),
                                       rhs.get_rows ());

  Eigen::Map<Eigen::VectorXd> uh (rhs.get_data (),
                                  rhs.get_rows ());

  Eigen::Map<const Eigen::MatrixXd> A (this->get_data (),
                                       this->get_rows (),
                                       this->get_cols ());
  
  uh = A.partialPivLu ().solve (f);
  
};

void
matrix::gauss_seidel
(const matrix &f,
 matrix &uh,
 const unsigned int maxit,
 const double tol)
{

  assert (uh.get_rows () == f.get_rows ());
  assert (uh.get_rows () == get_rows ());
  assert (uh.get_cols () == 1);
  
  double uh_new = 0;
  double incrnorm = 0;  
  
  for (unsigned int ii = 0; ii < maxit; ++ii)
    {
      incrnorm = 0;
      for (unsigned int jj = 0; jj < get_rows (); ++jj)
        {
          double res = f(jj, 0);

          for (unsigned int kk = 0; kk < get_cols (); ++kk)
            if (kk != jj)
              res -= const_index(jj,kk) * uh(kk, 0);
          
          uh_new = res / const_index(jj,jj);
          incrnorm = std::abs (uh_new - uh(jj, 0)) > incrnorm ?
            std::abs (uh_new - uh(jj, 0)) :
            incrnorm;
          uh(jj, 0) = uh_new;
        }
      if (incrnorm < tol)
        break;
    }
};
