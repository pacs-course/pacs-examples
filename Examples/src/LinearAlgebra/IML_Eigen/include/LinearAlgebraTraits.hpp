#ifndef HHH_LinearAlgebraTraits__HH
#define HHH_LinearAlgebraTraits__HH
#include <memory>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
namespace LinearAlgebra
{

  template <class Derived>
  inline double dot(const Eigen::SparseMatrixBase<Derived> &a,const
                    Eigen::SparseMatrixBase<Derived>& b)
  {
    return a.dot(b);
  }

  template <class Derived>
  inline double dot(const Eigen::MatrixBase<Derived> &a,const
                    Eigen::MatrixBase<Derived>& b)
  {
    return a.dot(b);
  }
  
  template <class Derived>
  inline double norm(const Eigen::SparseMatrixBase<Derived> & a)
  {
    return a.norm();
  }
  
  template <class Derived>
  inline double norm(const Eigen::MatrixBase<Derived> & a)
  {
    return a.norm();
  }

}//end namespace
#endif
