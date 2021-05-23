/*
 * test_SparseBlockMatrix.cpp
 *
 *  Created on: Jan 1, 2021
 *      Author: forma
 */

#include "SparseBlockMatrix.hpp"
#include <iostream>
#include <vector>
int main()
{
  using namespace apsc;
  using namespace Eigen;
  {
  SparseBlockMatrix<double,2,2> A({2,4},{2,4});
  // Block 0 0
  SparseBlockMatrix<double,2,2>::SpMat A00(A.rows({0,0}),A.cols({0,0}));
  using Tri=Eigen::Triplet<double>;
  std::vector<Tri> coeff;
  coeff.emplace_back(Tri{0,0,3.0});
  coeff.emplace_back(Tri{0,1,9.0});
  A00.setFromTriplets(coeff.begin(),coeff.end());
  A.setBlock({0,0},A00);
  // Block 0,1
  coeff.clear();
  coeff.emplace_back(Tri{0,0,1.0});
  coeff.emplace_back(Tri{0,1,2.0});
  coeff.emplace_back(Tri{1,0,3.0});
  A.getBlock({0,1}).setFromTriplets(coeff.begin(),coeff.end());
  // Block 1,0
  A.addTranspose({1,0},{0,1});
  // output
  std::cout<<"A00=\n"<<A.getBlock({0,0})<<std::endl;
  std::cout<<"A01=\n"<<A.getBlock({0,1})<<std::endl;
  std::cout<<"A10=\n"<<A.getBlock({1,0}).transpose()<<std::endl;
  std::cout<<"A11=\n"<<A.getBlock({1,1})<<std::endl;
  auto F=A.fullMatrix();
  std::cout<<"Full Matrix=\n"<<F<<std::endl;
  SparseBlockMatrix<double,2,2>::ColVector x;
  x.resize(A.cols());
  x=SparseBlockMatrix<double,2,2>::ColVector::Ones(A.cols());
  auto res = A*x;
  std::cout<<"res==\n"<<res<<std::endl;
  std::cout<<"with full\n"<<F*x<<std::endl;
  SparseBlockMatrix<double,2,2>::RowVector y;
  y=SparseBlockMatrix<double,2,2>::RowVector::Ones(A.rows());
  std::cout<<"res==\n"<<y*A<<std::endl;
  std::cout<<"with full\n"<<y*F<<std::endl;
  std::cout<<"Norm ="<< A.norm()<<std::endl;
  }
  //SparseBlockMatrix<double,2,2> A({2,4},{2,4});
  SparseBlockMatrix<double,2,2> A;
  A.resize({2,4},{2,4});
// Block 0 0
  SparseBlockMatrix<double,2,2>::SpMat A00(A.rows({0,0}),A.cols({0,0}));
  using Tri=Eigen::Triplet<double>;
  std::vector<Tri> coeff;
  coeff.emplace_back(Tri{0,0,3.0});
  coeff.emplace_back(Tri{0,1,9.0});
  A00.setFromTriplets(coeff.begin(),coeff.end());
  A.setBlock({0,0},A00);
  // Block 0,1
  coeff.clear();
  coeff.emplace_back(Tri{0,0,1.0});
  coeff.emplace_back(Tri{0,1,2.0});
  coeff.emplace_back(Tri{1,0,3.0});
  A.getBlock({0,1}).setFromTriplets(coeff.begin(),coeff.end());
  // Block 1,0
  A.addTranspose({1,0},{0,1});
  // output
  std::cout<<"A00=\n"<<A.getBlock({0,0})<<std::endl;
  std::cout<<"A01=\n"<<A.getBlock({0,1})<<std::endl;
  std::cout<<"A10=\n"<<A.getBlock({1,0}).transpose()<<std::endl;
  std::cout<<"A11=\n"<<A.getBlock({1,1})<<std::endl;
  auto F=A.fullMatrix();
  std::cout<<"Full Matrix=\n"<<F<<std::endl;
  SparseBlockMatrix<double,2,2>::ColVector x;
  x.resize(A.cols());
  x=SparseBlockMatrix<double,2,2>::ColVector::Ones(A.cols());
  auto res = A*x;
  std::cout<<"res==\n"<<res<<std::endl;
  std::cout<<"with full\n"<<F*x<<std::endl;
  SparseBlockMatrix<double,2,2>::RowVector y;
  y=SparseBlockMatrix<double,2,2>::RowVector::Ones(A.rows());
  std::cout<<"res==\n"<<y*A<<std::endl;
  std::cout<<"with full\n"<<y*F<<std::endl;
  std::cout<<"Norm ="<< A.norm()<<std::endl;

  auto B=A;
  // output
   std::cout<<"b00=\n"<<B.getBlock({0,0})<<std::endl;
   std::cout<<"b01=\n"<<B.getBlock({0,1})<<std::endl;
   std::cout<<"b10=\n"<<B.getBlock({1,0}).transpose()<<std::endl;
   std::cout<<"b11=\n"<<B.getBlock({1,1})<<std::endl;
   F=B.fullMatrix();
   std::cout<<"Full Matrix=\n"<<F<<std::endl;
   SparseBlockMatrix<double,2,2>::ColVector xx;
   xx.resize(B.cols());
   xx=SparseBlockMatrix<double,2,2>::ColVector::Ones(A.cols());
   res = B*xx;
   std::cout<<"res==\n"<<res<<std::endl;
   std::cout<<"with full\n"<<F*xx<<std::endl;
   y=SparseBlockMatrix<double,2,2>::RowVector::Ones(A.rows());
   std::cout<<"res==\n"<<y*B<<std::endl;
   std::cout<<"with full\n"<<y*F<<std::endl;
   std::cout<<"Norm ="<< B.norm()<<std::endl;

}


