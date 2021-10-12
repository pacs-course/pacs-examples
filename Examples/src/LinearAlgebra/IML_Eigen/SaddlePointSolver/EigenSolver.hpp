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
#include <Spectra/MatOp/SparseGenMatProd.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/SymEigsSolver.h>
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
namespace LinearAlgebra
{
/*!
 * Computes the eigenvalues of a sparse EIgen matrix using the tools provided by
 * the SPECTRA library
 * @tparam SpMat The sparse matrix
 * @tparam SelectionRule  Which eigenvalue you prefer, according to the enum
 * specified in spectra
 * @param M The matrix
 * @param numEigs The desired number of eigenvectors
 * @return A vector of complex numbers containing the eigenvalues
 */
template <typename SpMat,
          Spectra::SortRule SelectionRule = Spectra::SortRule::BothEnds>
Eigen::VectorXcd
computeEigenValues(SpMat const &M, int numEigs)
{
  Eigen::VectorXcd                                           evalues;
  Spectra::SparseGenMatProd<double>                          op(M);
  Spectra::GenEigsSolver<Spectra::SparseGenMatProd<double> > eigs{
    op, numEigs, 2 * numEigs + 1};
  eigs.init();
  eigs.compute(SelectionRule, 2500, 1.e-6);
  if(eigs.info() != Spectra::CompInfo::Successful)
    {
      std::cerr << "Eigenvalue computation not successful";
    }
  else
    evalues = eigs.eigenvalues();
  return evalues;
}

/*!
 * Computes the eigenvalues of a symmetric EIgen matrix using the tools provided
 * by the SPECTRA library
 *
 * @tparam SpMat The Eigen matrix type
 * @tparam SelectionRule Which eigenvalue you prefer, according to the enum
 * specified in spectra
 * @param M The matrix
 * @param numEigs The number of eigenvalues requested
 * @param ncv The number of vectors for the Arnoldi iteration
 * @param maxit The maximum number of iterations
 * @param tol A tolerance
 * @return A Eigen vector of doubles with the eigenvalues
 */
template <typename SpMat,
          Spectra::SortRule SelectionRule = Spectra::SortRule::BothEnds>
Eigen::VectorXd
computeSymEigenValues(SpMat const &M, int numEigs, long int ncv = 20L,
                      long int maxit = 1500L, double tol = 1.e-08)
{
  Eigen::VectorXd                   evalues;
  Spectra::SparseSymMatProd<double> op(M);

  Spectra::SymEigsSolver<Spectra::SparseSymMatProd<double> > eigs(op, numEigs,
                                                                  ncv);
  eigs.init();
  eigs.compute(SelectionRule, maxit, tol);
  auto status = eigs.info();
  if(status == Spectra::CompInfo::Successful)
    {
      evalues = eigs.eigenvalues();
    }
  else if(status == Spectra::CompInfo::NotConverging)
    {
      std::cerr << "Eigenvalue computation not converged.\n ";
      evalues = eigs.eigenvalues();
    }
  else
    {
      std::cerr << "Eigenvalue computation not successful.\n ";
    }
  return evalues;
}
} // namespace LinearAlgebra

#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_EIGENSOLVER_HPP_ \
        */
