/*
 * main_matrix.cpp
 *
 *  Created on: Oct 14, 2022
 *      Author: forma
 */
#include <iostream>
#include "Matrix.hpp"
int main()
{
  using namespace apsc::LinearAlgebra;
  std::cout<<"Creating a 6 by 6 Matrix of double, row-major\n";
  apsc::LinearAlgebra::Matrix<double,ORDERING::ROWMAJOR> mr(6,6);
  mr.fillRandom();
  std::cout<<mr;
  // a vector of ones
  std::vector<double> ones(6,1.);
  auto res = mr*ones;
  std::cout<<"Matrix times 1=\n";
  for (auto v:res)
    std::cout<<v<<" ";
  std::cout<<std::endl;
  std::cout<<"Creating a 6 by 6 Matrix of double, column-major\n";
  apsc::LinearAlgebra::Matrix<double,ORDERING::COLUMNMAJOR> mc(6,6);
  for (std::size_t i=0u;i<mc.rows()*mc.cols();++i)
    {
      mc.data()[i]=mr.data()[i];// A trick
    }
  std::cout<<mc;
  res=mr*ones;
  std::cout<<"Matrix times 1=\n";
  for (auto v:res)
    std::cout<<v<<" ";
  std::cout<<std::endl;
  std::cout<<"Creating a 4 by 6 Matrix of int, row-major\n";
   apsc::LinearAlgebra::Matrix<int,ORDERING::ROWMAJOR> mi(4,6);
   mi.fillRandom();
   std::cout<<mi;





}


