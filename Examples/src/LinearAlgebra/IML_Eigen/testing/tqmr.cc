#include <cstdlib>                         // System includes
#include <iostream>                      // 
using std::endl;
using std::cin;
using std::cout;
using std::cerr;

//#include <iostream.h>                       // 


#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include "MM_readers.hpp"

#include "qmr.hpp"                            // IML++ QMR template

int
main(int argc, char * argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " HBfile " << endl;
    exit(-1);
  }

  // Some useful alias
  using namespace LinearAlgebra;
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;

  double tol = 1.e-6;                       // Convergence tolerance
  int result, maxit = 1500;                  // Maximum iterations

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
  Eigen::DiagonalPreconditioner<double> D1(A);// Create diagonal preconditioner
  Eigen::IncompleteLUT<double> D;
  D.setDroptol(0.);
  D.setFillfactor(4);
  D.compute(A);
  x=0*x;

  result = QMR(A, x, b, D1, D, maxit, tol);  // Solve system

  cout << "QMR flag = " << result << endl;
  cout << "iterations performed: " << maxit << endl;
  cout << "tolerance achieved  : " << tol << endl;
  cout << "Error:                " << (x-e).norm()<<std::endl;


  return result;
}
