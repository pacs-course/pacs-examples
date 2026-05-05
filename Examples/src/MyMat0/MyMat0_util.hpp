#ifndef HH_MYMAT0_UTIL__HH
#define HH_MYMAT0_UTIL__HH
#include "MyMat0.hpp"
#include <numeric>
namespace LinearAlgebra
{
//! Classic dense matrix-matrix multiplication.
/*!
  This implementation uses the straightforward triple loop.

  @param m1 Left-hand-side matrix.
  @param m2 Right-hand-side matrix.
  @return A row-major matrix containing the product `m1*m2`.
  @throws std::invalid_argument if the matrix sizes are incompatible.
 */
template <typename T, StoragePolicySwitch storagePolicy1,
          StoragePolicySwitch storagePolicy2>
MyMat0<T, ROWMAJOR>
matMul(MyMat0<T, storagePolicy1> const &m1, MyMat0<T, storagePolicy2> const &m2)
{
  if(m1.ncol() != m2.nrow())
    throw std::invalid_argument("Incompatible matrix sizes in matMul");
  MyMat0<T, ROWMAJOR> res(m1.nrow(), m2.ncol(), T{});
  for(size_type i = 0; i < m1.nrow(); ++i)
    for(size_type j = 0; j < m2.ncol(); ++j)
      for(size_type k = 0; k < m2.nrow(); ++k)
// if c++23 I can use [k,j] instead of (k,j)
#if __cplusplus >= 202100L
        res(i, j) += m1[i, k] * m2[k, j];
#else
        res(i, j) += m1(i, k) * m2(k, j);
#endif
  return res;
}

/*!\defgroup matmat Optimized matrix-matrix multiplication
  These overloads reorganize the loops according to the storage layout of the
  operands in order to improve locality of reference.
  @{
*/
//! Optimized multiplication for row-major times row-major matrices.
/*!
  The implementation caches one column of the right-hand-side matrix and
  combines it with each row of the left-hand-side matrix using
  `std::inner_product`.

  @param m1 Left-hand-side row-major matrix.
  @param m2 Right-hand-side row-major matrix.
  @return A row-major matrix containing the product `m1*m2`.
  @throws std::invalid_argument if the matrix sizes are incompatible.
*/
template <typename T>
MyMat0<T, ROWMAJOR>
matMulOpt(MyMat0<T, ROWMAJOR> const &m1, MyMat0<T, ROWMAJOR> const &m2)
{
  if(m1.ncol() != m2.nrow())
    throw std::invalid_argument("Incompatible matrix sizes in matMulOpt");
  MyMat0<T, ROWMAJOR> res(m1.nrow(), m2.ncol(), T{});
  std::vector<T>      tmp(m2.nrow());
  for(size_type j = 0; j < m2.ncol(); ++j)
    {
      // Store the jth column of m2
      for(size_type l = 0; l < m2.nrow(); ++l)
        {
          tmp[l] = m2(l, j);
        }
      for(size_type i = 0; i < m1.nrow(); ++i)
        {
          size_type row_starts = m1.getIndex(i, 0);
          // ! can use parallel version! it is called transform_reduce
          res(i, j) = std::inner_product(tmp.begin(), tmp.end(),
                                         m1.cbegin() + row_starts, T(0));
          /*
          for(size_type k=0; k<m2.nrow();++k)
            {
              res(i,j) += m1(i,k)*tmp[k];
              }
          */
        }
    }
  return res;
}

//! Optimized multiplication for column-major times row-major matrices.
/*!
  This is the least cache-friendly combination for the implemented strategy, so
  the left-hand-side matrix is first copied into a temporary row-major matrix.

  @param m1 Left-hand-side column-major matrix.
  @param m2 Right-hand-side row-major matrix.
  @return A row-major matrix containing the product `m1*m2`.
  @throws std::invalid_argument if the matrix sizes are incompatible.
 */
template <typename T>
MyMat0<T, ROWMAJOR>
matMulOpt(MyMat0<T, COLUMNMAJOR> const &m1, MyMat0<T, ROWMAJOR> const &m2)
{
  // I need a temporary matrix. This is very costly memory-wise
  // but I do not have alternatives here
  auto mc1 = MyMat0<T, ROWMAJOR>(m1);
  return matMulOpt(mc1, m2);
}

//! Optimized multiplication for column-major times column-major matrices.
/*!
  Each row of the left-hand-side matrix is copied into a temporary buffer and
  then multiplied by contiguous columns of the right-hand-side matrix.

  @param m1 Left-hand-side column-major matrix.
  @param m2 Right-hand-side column-major matrix.
  @return A row-major matrix containing the product `m1*m2`.
  @throws std::invalid_argument if the matrix sizes are incompatible.
 */
template <typename T>
MyMat0<T, ROWMAJOR>
matMulOpt(MyMat0<T, COLUMNMAJOR> const &m1, MyMat0<T, COLUMNMAJOR> const &m2)
{
  if(m1.ncol() != m2.nrow())
    throw std::invalid_argument("Incompatible matrix sizes in matMulOpt");
  MyMat0<T, ROWMAJOR> res(m1.nrow(), m2.ncol(), T{});
  std::vector<T>      tmp(m1.ncol());

  for(size_type i = 0; i < m1.nrow(); ++i)
    {
      // Store the i-th row of m1
      for(size_type l = 0; l < m1.ncol(); ++l)
        {
          tmp[l] = m1(i, l);
        }
      for(size_type j = 0; j < m2.ncol(); ++j)
        {
          size_type column_start = m2.getIndex(0, j);
          //! I can parallelize here!
          res(i, j) = std::inner_product(tmp.begin(), tmp.end(),
                                         m2.cbegin() + column_start, T(0));
          /*
            for(size_type k=0; k<m2.nrow();++k)
            {
              res(i,j) += tmp[k]*m2(k,j);
            }
          */
        }
    }
  return res;
}

//! Optimized multiplication for row-major times column-major matrices.
/*!
  This is the most favorable combination for the implemented strategy because
  both the selected row of `m1` and the selected column of `m2` are contiguous
  in memory.

  @param m1 Left-hand-side row-major matrix.
  @param m2 Right-hand-side column-major matrix.
  @return A row-major matrix containing the product `m1*m2`.
  @throws std::invalid_argument if the matrix sizes are incompatible.
 */
template <typename T>
MyMat0<T, ROWMAJOR>
matMulOpt(MyMat0<T, ROWMAJOR> const &m1, MyMat0<T, COLUMNMAJOR> const &m2)
{
  if(m1.ncol() != m2.nrow())
    throw std::invalid_argument("Incompatible matrix sizes in matMulOpt");
  MyMat0<T, ROWMAJOR> res(m1.nrow(), m2.ncol(), T{});
  for(size_type i = 0; i < m1.nrow(); ++i)
    {
      for(size_type j = 0; j < m2.ncol(); ++j)
        {
          size_type column_start = m2.getIndex(0, j);
          size_type row_start = m1.getIndex(i, 0);
          size_type row_end = m1.getIndex(i + 1, 0);
          // I can parallelize here!
          res(i, j) =
            std::inner_product(m1.cbegin() + row_start, m1.cbegin() + row_end,
                               m2.cbegin() + column_start, T(0));
        }
    }
  /*
  for(size_type k=0; k<m2.nrow();++k)
  res(i,j) += m1(i,k)*m2(k,j);
  */
  return res;
}

/*! @}*/

/*!\defgroup matmatblas BLAS-based optimized matrix-matrix multiplication
  These overloads delegate the inner products to the BLAS routine
  `cblas_ddot`, so they are available only for `double`.
  @{
*/
#ifndef NOBLAS
//! BLAS-assisted multiplication for row-major times row-major matrices.
MyMat0<double, ROWMAJOR> matMulOptBlas(MyMat0<double, ROWMAJOR> const &m1,
                                       MyMat0<double, ROWMAJOR> const &m2);

//! BLAS-assisted multiplication for column-major times row-major matrices.
MyMat0<double, ROWMAJOR> matMulOptBlas(MyMat0<double, COLUMNMAJOR> const &m1,
                                       MyMat0<double, ROWMAJOR> const    &m2);

//! BLAS-assisted multiplication for column-major times column-major matrices.
MyMat0<double, ROWMAJOR> matMulOptBlas(MyMat0<double, COLUMNMAJOR> const &m1,
                                       MyMat0<double, COLUMNMAJOR> const &m2);

//! BLAS-assisted multiplication for row-major times column-major matrices.
MyMat0<double, ROWMAJOR> matMulOptBlas(MyMat0<double, ROWMAJOR> const    &m1,
                                       MyMat0<double, COLUMNMAJOR> const &m2);
#endif
/*! @}*/

} // namespace LinearAlgebra

#endif
