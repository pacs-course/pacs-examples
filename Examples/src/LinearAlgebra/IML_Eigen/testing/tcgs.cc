#include <cstdlib>                      // System includes
//#include <iostream.h>                    // 
#include <iostream>                      // 
using std::endl;
using std::cin;
using std::cout;
using std::cerr;


#include <iostream>                      // 
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include "MM_readers.hpp"
#include "cgs.hpp"                         // IML++ CGS template

int
main(int argc, char * argv[])
{
  using namespace LinearAlgebra;
  // Some useful alias
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " HBfile " << endl;
    exit(-1);
  }

  double tol = 1.e-6;                    // Convergence tolerance
  int result, maxit = 150;               // Maximum iterations
  std::string matrixFile(argv[1]);
  SpMat A=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  std::cout<<"Matrix size:"<<A.rows()<<"X"<<A.cols()<<std::endl;
  std::cout<<"Non zero entries:"<<A.nonZeros()<<std::endl;
  SpMat B = SpMat(A.transpose()) - A;
  std::cout<<"Norm of A-A.t: "<<B.norm()<<std::endl;
  SpVec e=SpVec::Ones(A.rows());
  SpVec b=A* e;
  SpVec x(A.rows());
  //Eigen::IdentityPreconditioner D(A);// Create diagonal preconditioner
  Eigen::DiagonalPreconditioner<double> D(A);// Create diagonal preconditioner
  x=0*x;


  result = CGS(A, x, b, D, maxit, tol); // Solve system

  cout << "CGS   flag = " << result << endl;
  cout << "iterations performed: " << maxit << endl;
  cout << "tolerance achieved  : " << tol << endl;
  cout << "Error:                " << (x-e).norm()<<std::endl;

  return result;
}
