#ifndef NOBLAS
#include "MyMat0_util.hpp"
#include <cblas.h> // the blas
namespace LinearAlgebra
{
//! BLAS-assisted multiplication for row-major times row-major matrices.
/*!
  A temporary buffer stores one column of the right-hand-side matrix so that
  the dot product can be passed to `cblas_ddot`.
 */
MyMat0<double, ROWMAJOR>
matMulOptBlas(MyMat0<double, ROWMAJOR> const &m1,
              MyMat0<double, ROWMAJOR> const &m2)
{
  MyMat0<double, ROWMAJOR> res(m1.nrow(), m2.ncol(), 0.);
  std::vector<double>      tmp(m2.nrow());
  for(size_type j = 0; j < m2.ncol(); ++j)
    {
      // Store the jth column of m2
      for(size_type l = 0; l < m2.nrow(); ++l)
        {
          tmp[l] = m2(l, j);
        }
      for(size_type i = 0; i < m1.nrow(); ++i)
        {
          double const *z = &m1(i, 0);
          res(i, j) = cblas_ddot(m2.nrow(), tmp.data(), 1, z, 1);
        }
    }
  return res;
}

//! BLAS-assisted multiplication for column-major times row-major matrices.
/*!
  The left-hand-side matrix is first copied into row-major storage and then the
  row-major overload is reused.
 */
MyMat0<double, ROWMAJOR>
matMulOptBlas(MyMat0<double, COLUMNMAJOR> const &m1,
              MyMat0<double, ROWMAJOR> const    &m2)
{
  // I need a temporary matrix. doublehis is very costly memoriwise
  // but I do not have alternatives here
  auto mc1 = MyMat0<double, ROWMAJOR>(m1);
  return matMulOptBlas(mc1, m2);
}

//! BLAS-assisted multiplication for column-major times column-major matrices.
/*!
  Each row of the left-hand-side matrix is copied into a temporary buffer and
  multiplied by contiguous columns of the right-hand-side matrix.
 */
MyMat0<double, ROWMAJOR>
matMulOptBlas(MyMat0<double, COLUMNMAJOR> const &m1,
              MyMat0<double, COLUMNMAJOR> const &m2)
{
  MyMat0<double, ROWMAJOR> res(m1.nrow(), m2.ncol(), 0.);
  std::vector<double>      tmp(m1.ncol());

  for(size_type i = 0; i < m1.nrow(); ++i)
    {
      // Store the i-th row of m1
      for(size_type l = 0; l < m1.ncol(); ++l)
        {
          tmp[l] = m1(i, l);
        }
      for(size_type j = 0; j < m2.ncol(); ++j)
        {
          double const *z = &m2(0, j);
          res(i, j) = cblas_ddot(m1.ncol(), tmp.data(), 1, z, 1);
        }
    }
  return res;
}

//! BLAS-assisted multiplication for row-major times column-major matrices.
/*!
  This case can pass both operands directly to `cblas_ddot` because the row of
  the left-hand-side matrix and the column of the right-hand-side matrix are
  already contiguous.
 */
MyMat0<double, ROWMAJOR>
matMulOptBlas(MyMat0<double, ROWMAJOR> const    &m1,
              MyMat0<double, COLUMNMAJOR> const &m2)
{
  MyMat0<double, ROWMAJOR> res(m1.nrow(), m2.ncol(), 0.);
  for(size_type i = 0; i < m1.nrow(); ++i)
    {
      for(size_type j = 0; j < m2.ncol(); ++j)
        {
          double const *z1 = &m1(i, 0);
          double const *z2 = &m2(0, j);
          res(i, j) = cblas_ddot(m2.nrow(), z1, 1, z2, 1);
        }
    }
  return res;
}
} // namespace LinearAlgebra
#endif
