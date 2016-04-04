#include "MyMat0_util.hpp"
#include <cblas.h> // the blas
namespace LinearAlgebra
{
  /*!\defgroup matmat otimized matrix-matrix computation.

   @{*
   
   Chache friendly computations. I avoid striding along columns on a ROWMAJOR matrix
  */
  MyMat0<double,ROWMAJOR> matMulOptBlas(
                                    MyMat0<double,ROWMAJOR> const & m1,
                                    MyMat0<double,ROWMAJOR> const & m2)
  {
    MyMat0<double,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    std::vector<double> tmp(m2.nrow());
    for(size_type j=0; j<m2.ncol();++j)
      {
        // Store the jth column of m2
        for (size_type l=0; l<m2.nrow(); ++l)
          {
            tmp[l]=m2(l,j);
          }
        for(size_type i=0; i<m1.nrow();++i)
          {
            double const * z = &m1(i,0);
            res(i,j) = cblas_ddot(m2.nrow(),tmp.data(),1,z,1);
          }
      }
    return res;
  }

  /*! doublehe worst case
   */

  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,COLUMNMAJOR> const & m1,MyMat0<double,ROWMAJOR> const & m2)
  {
    // I need a temporary matrix. doublehis is very costly memoriwise
    // but I do not have alternatives here
    auto mc1=MyMat0<double,ROWMAJOR>(m1);
    return matMulOptBlas(mc1,m2);
  }

  /*! An intermediate case
   */
  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,COLUMNMAJOR> const & m1,MyMat0<double,COLUMNMAJOR> const & m2)
  {
    MyMat0<double,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    std::vector<double> tmp(m1.ncol());

    for(size_type i=0; i<m1.nrow();++i)
      {
        // Store the i-th row of m1
        for (size_type l=0; l<m1.ncol(); ++l)
          {
            tmp[l]=m1(i,l);
          }
        for(size_type j=0; j<m2.ncol();++j)
          {
            double const * z = &m2(0,j);
            res(i,j) = cblas_ddot(m1.ncol(),tmp.data(),1,z,1);
          }
      }
    return res;
    
  }

  MyMat0<double,ROWMAJOR> matMulOptBlas(MyMat0<double,ROWMAJOR> const & m1,MyMat0<double,COLUMNMAJOR> const & m2)
  {
    MyMat0<double,ROWMAJOR> res(m1.nrow(),m2.ncol(),0.);
    for(size_type i=0; i<m1.nrow();++i)
      {
        for(size_type j=0; j<m2.ncol();++j)
          {
            double const * z1 = &m1(i,0);
            double const * z2 = &m2(0,j);          
            res(i,j) = cblas_ddot(m2.nrow(),z1,1,z2,1);
          }
      }
    return res;
 
  }

  /*! @}*/

}
