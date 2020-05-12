#include <cstdlib>                      // System includes
//#include <iostream.h>                    // 
#include <iostream>                      // 
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include "MM_readers.hpp"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

#include "cg.hpp"                          // IML++ CG template

int
main(int argc, char * argv[])
{
  using namespace LinearAlgebra;
  // Some useful alias
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " input file " << endl;
    exit(-1);
  }
  std::string matrixFile(argv[1]);

  double tol = 1.e-6;                    // Convergence tolerance
  int result, maxit = 15000;               // Maximum iterations
  SpMat A=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  std::cout<<"Matrix size:"<<A.rows()<<"X"<<A.cols()<<std::endl;
  std::cout<<"Non zero entries:"<<A.nonZeros()<<std::endl;
  SpMat B = SpMat(A.transpose()) - A;
  std::cout<<"Norm of A-A.t: "<<B.norm()<<std::endl;
  //  SpMat A(1000,1000);
  //A.reserve(1000);
  //for (int i=0;i<1000;++i)A.coeffRef(i,i)=i;
  // Create Rhs b
  SpVec e=SpVec::Ones(A.rows());
  SpVec b=A* e;
  SpVec x(A.rows());
  //Eigen::IdentityPreconditioner D(A);// Create diagonal preconditioner
  Eigen::DiagonalPreconditioner<double> D(A);// Create diagonal preconditioner

  // First with eigen cg
  Eigen::ConjugateGradient<SpMat, Eigen::Lower|Eigen::Upper> cg;
  cg.setMaxIterations(maxit);
  cg.setTolerance(tol);
  cg.compute(A);
  x = cg.solve(b);
  std::cout <<" Eigen native CG"<<std::endl;
  std::cout << "#iterations:     " << cg.iterations() << std::endl;
  std::cout << "estimated error: " << cg.error()      << std::endl;
  std::cout << "effective error: "<<(x-e).norm()<<std::endl;
  // Now with IML++
  x=0*x;
  result = CG(A, x, b, D, maxit, tol);   // Solve system

  std::cout <<" IML++ CG "<<std::endl;
  cout << "CG flag = " << result << endl;
  cout << "iterations performed: " << maxit << endl;
  cout << "tolerance achieved  : " << tol << endl;
  std::cout << "Error norm: "<<(x-e).norm()<<std::endl;

  return result;
}
