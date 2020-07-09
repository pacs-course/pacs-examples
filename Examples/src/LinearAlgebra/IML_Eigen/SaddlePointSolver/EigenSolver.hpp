/*
 * EigenSolver.hpp
 *
 *  Created on: May 13, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_EIGENSOLVER_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_EIGENSOLVER_HPP_
#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <iostream>
/*enum  	Spectra::SELECT_EIGENVALUE {
  Spectra::LARGEST_MAGN = 0,
  Spectra::LARGEST_REAL,
  Spectra::LARGEST_IMAG,
  Spectra::LARGEST_ALGE,
  Spectra::SMALLEST_MAGN,
  Spectra::SMALLEST_REAL,
  Spectra::SMALLEST_IMAG,
  Spectra::SMALLEST_ALGE,
  Spectra::BOTH_ENDS
}*/
namespace LinearAlgebra{

template <typename SpMat, int SelectionRule = Spectra::BOTH_ENDS >
Eigen::VectorXcd computeEigenValues(SpMat const & M, int numEigs)
{
  Eigen::VectorXcd evalues;
  Spectra::SparseGenMatProd<double> op(M);
  Spectra::GenEigsSolver< double, SelectionRule, Spectra::SparseGenMatProd<double> > eigs(&op, numEigs, 2*numEigs+1);
  eigs.init();
  eigs.compute(2500,1.e-6);
  if(eigs.info() != Spectra::SUCCESSFUL)
    {
      std::cerr<<"Eigenvalue computation not successful";
    }
  else
    evalues=eigs.eigenvalues();
  return evalues;
}

template <typename SpMat, int SelectionRule = Spectra::BOTH_ENDS >
Eigen::VectorXd computeSymEigenValues(SpMat const & M, int numEigs, long int ncv=20L, long int maxit=1500L, double tol=1.e-08)
{
  Eigen::VectorXd evalues;
  Spectra::SparseSymMatProd<double> op(M);

  Spectra::SymEigsSolver< double, SelectionRule, Spectra::SparseSymMatProd<double> >
  eigs(&op, numEigs, ncv);
  eigs.init();
  eigs.compute(maxit,tol);
  auto status=eigs.info();
  if(status == Spectra::SUCCESSFUL)
    {
      evalues=eigs.eigenvalues();
    }
  else if (status== Spectra::NOT_CONVERGING)
    {
      std::cerr<<"Eigenvalue computation not converged.\n ";
      evalues=eigs.eigenvalues();
    }
  else
    {
         std::cerr<<"Eigenvalue computation not successful.\n ";
    }
  return evalues;
}
}// end namespace

#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_EIGENSOLVER_HPP_ */
