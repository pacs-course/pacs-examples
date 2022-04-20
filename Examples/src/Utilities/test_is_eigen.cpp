/*
 * test_is_complex.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "is_eigen.hpp"
#include <iostream>
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
  std::cout << c << " is eigen= (should be false)" << std::boolalpha << is_eigen<decltype(c)>()
            << std::endl;
  std::cout << A << " is eigen= (should be true)" << std::boolalpha << is_eigen<decltype(A)>()
            << std::endl;
  // Now using the traits value directly
  std::cout << c << " is eigen= (should be false)" << std::boolalpha
            << is_eigen_v<decltype(c)> << std::endl;
  std::cout << A << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(A)> << std::endl;
  std::cout << smat << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(smat)> << std::endl;
  std::cout << svec << " is eigen= (should be true)" << std::boolalpha
            << is_eigen_v<decltype(svec)> << std::endl;
}
