/*
 * test_is_complex.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "is_eigen.hpp"
#include <iostream>
// using concepts to test if template argument is an eigen matrix
template <apsc::TypeTraits::EigenMatrixType M>
void
fun(M const &)
{
  std::cout<<"fun() called with an Eigen matrix"<<std::endl;
}
// example of everloading with concepts
template <apsc::TypeTraits::EigenSparseMatrixType M>
void
fun2(M const &)
{
  std::cout<<"fun2() called with a Sparse Eigen matrix"<<std::endl;
}
template <apsc::TypeTraits::EigenDenseMatrixType M>
void
fun2(M const &)
{
  std::cout<<"fun2() called with a Dense Eigen matrix"<<std::endl;
}


int
main()
{
  using namespace apsc::TypeTraits;
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;
  SpMat smat(5,5);
  SpVec svec(5);
  svec=SpVec::Ones(5);
  Eigen::Matrix<double, 2, 2> A;
  A << 1, 2, 3, 4;
  double c = 9.0;
  // Using the traits value directly
  std::cout << " is eigen= (should be false)" << std::boolalpha
            << is_eigen_v<decltype(c)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(A)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(smat)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(svec)> << std::endl;
  std::cout << " is dense eigen= (should be false)" << std::boolalpha
             << is_dense_eigen_v<decltype(smat)> << std::endl;
  std::cout << " is sparse eigen= (should be true)" << std::boolalpha
              << is_sparse_eigen_v<decltype(smat)> << std::endl;
    // Using concepts as traits
  std::cout << " is eigen= (should be false)" << std::boolalpha
            << EigenMatrixType<decltype(c)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << EigenMatrixType<decltype(A)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << EigenMatrixType<decltype(smat)> << std::endl;
  std::cout << " is eigen= (should be true)" << std::boolalpha
            << EigenMatrixType<decltype(svec)> << std::endl;

  fun(svec);
  fun(A);
  //fun(c); // it fails compilation
  fun2(smat);
  fun2(A);

}
