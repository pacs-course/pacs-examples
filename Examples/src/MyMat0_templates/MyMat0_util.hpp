#ifndef HH_MYMAT0_UTIL__HH
#define HH_MYMAT0_UTIL__HH
#include "MyMat0.hpp"
#include <numeric>
namespace LinearAlgebra
{
  //! Classic matrix-matrix multiplication
  /*
    Note that no checks on consistency of dimensions is made. It is up to the user
    to make sure that the number of columns of m1 is equal to the number of rows of
    m2. Failure to do so will probably lead to a segfault!

    \param m1 lhs matrix
    \param m2 rhs matrix
    \result A rowmajor matrix containing the product m1*m2
   */
  template<typename T, StoragePolicySwitch storagePolicy1, StoragePolicySwitch storagePolicy2>
  MyMat0<T,ROWMAJOR> matMul(MyMat0<T,storagePolicy1> const & m1,MyMat0<T,storagePolicy2> const & m2)
  {
    MyMat0<T,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    for(size_type i=0; i<m1.nrow();++i)
      for(size_type j=0; j<m2.ncol();++j)
        for(size_type k=0; k<m2.nrow();++k)
          res(i,j) += m1(i,k)*m2(k,j);
    return res;
  }
  
  /*!\defgroup matmat otimized matrix-matrix computation.

   @{*
   
   Chache friendly computations. I avoid striding along columns on a ROWMAJOR matrix
  */
  template <typename T>
  MyMat0<T,ROWMAJOR> matMulOpt(MyMat0<T,ROWMAJOR> const & m1,MyMat0<T,ROWMAJOR> const & m2)
  {
    
    MyMat0<T,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    std::vector<T> tmp(m2.nrow());
    for(size_type j=0; j<m2.ncol();++j)
      {
        // Store the jth column of m2
        for (size_type l=0; l<m2.nrow(); ++l)
          {
            tmp[l]=m2(l,j);
          }
        for(size_type i=0; i<m1.nrow();++i)
          {
            size_type row_starts= m1.getIndex(i,0);
            res(i,j) = std::inner_product(tmp.begin(),tmp.end(),m1.cbegin()+row_starts,T(0));
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

  /*! The worst case
   */
  template <typename T>
  MyMat0<T,ROWMAJOR> matMulOpt(MyMat0<T,COLUMNMAJOR> const & m1,MyMat0<T,ROWMAJOR> const & m2)
  {
    // I need a temporary matrix. This is very costly memoriwise
    // but I do not have alternatives here
    auto mc1=MyMat0<T,ROWMAJOR>(m1);
    return matMulOpt(mc1,m2);
  }

  /*! An intermediate case
   */
  template <typename T>
  MyMat0<T,ROWMAJOR> matMulOpt(MyMat0<T,COLUMNMAJOR> const & m1,MyMat0<T,COLUMNMAJOR> const & m2)
  {
    MyMat0<T,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    std::vector<T> tmp(m1.ncol());

    for(size_type i=0; i<m1.nrow();++i)
      {
        // Store the i-th row of m1
        for (size_type l=0; l<m1.ncol(); ++l)
          {
            tmp[l]=m1(i,l);
          }
        for(size_type j=0; j<m2.ncol();++j)
          {
            size_type column_start = m2.gitIndex(0,j);
            res(i,j) = std::inner_product(tmp.begin(),tmp.end(),m2.cbegin()+column_start,T(0));
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
  
  /*! 
    The simplest case
   */
  template <typename T>
  MyMat0<T,ROWMAJOR> matMulOpt(MyMat0<T,ROWMAJOR> const & m1,MyMat0<T,COLUMNMAJOR> const & m2)
  {
    MyMat0<T,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    for(size_type i=0; i<m1.nrow();++i)
      {
        for(size_type j=0; j<m2.ncol();++j)
          {
            size_type column_start = m2.getIndex(0,j);
            size_type row_start = m1.getIndex(i,0);
            size_type row_end   = m1.getIndex(i+1,0);
            res(i,j) = std::inner_product(
                                          m1.cbegin()+row_start,
                                          m1.cbegin()+row_end,
                                          m2.cbegin()+column_start,
                                          T(0));
          }
      }
        /*
        for(size_type k=0; k<m2.nrow();++k)
        res(i,j) += m1(i,k)*m2(k,j);
        */
    return res;
  }
  
  /*! @}*/


  /*!\defgroup matmatblas Optimised for blas (only for T=double)
    
    @{*

    Cache friendly computations. Using blas for dot product
  */
  
  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,ROWMAJOR> const & m1,MyMat0<double,ROWMAJOR> const & m2);

  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,COLUMNMAJOR> const & m1,MyMat0<double,ROWMAJOR> const & m2);

  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,COLUMNMAJOR> const & m1,MyMat0<double,COLUMNMAJOR> const & m2);

  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,ROWMAJOR> const & m1,MyMat0<double,COLUMNMAJOR> const & m2);

  /*! @}*/

}

#endif
