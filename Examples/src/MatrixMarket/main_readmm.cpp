/*! This file provides an example of utility to read matrix marked
  matrices into eigen matrices
*/
#include <iostream>
#include "EigenReadMM.hpp"
#include <Eigen/SparseCore>
int main(){
  Eigen::SparseMatrix<double> mat;
  std::string filename;
  //  std::cout<<"Give me the name of the matrix marked mtx file"<<std::endl;
  //std::cin>>filename;
  filename="data/e05r0500.mtx";
  EigenReaders::readFromMMFile(mat,filename);
  std::cout<<"Matrix read. Ncol= "<<mat.cols()<<" Nrows="<<mat.rows()<<
    " Non zero entries="<<mat.nonZeros()<<std::endl;
}
