/*
 * dumpMatrices.hpp
 *
 *  Created on: Dec 4, 2018
 *      Author: forma
 */

#ifndef SRC_FVCODE3D_UTILITY_DUMPMATRICES_HPP_
#define SRC_FVCODE3D_UTILITY_DUMPMATRICES_HPP_
#include "preconditioner.hpp"
#include <unsupported/Eigen/SparseExtra>
#include <iosfwd>
#include <string>
namespace FVCode3D
{
//! Dumps a sparse matrix
/*!
 *  It is just a wrapper to the saveMarket utility of the Eigen
 *  Since it is still unsupported I prefer to hide it into a wrapper
 *  @param M a Eigen sparse matrix
 *  @param filename Where the matrix will be stored
 *  @param sym true if the matrix is symmetric. The matrix market file will
 * store only the lower dimensional part
 *  @return false if something wrong happened
 */
template <typename SparseMat>
bool
dumpSparseMatrix(const SparseMat &M, const std::string &filename,
                 bool sym = false)
{
  return Eigen::saveMarket(M, filename, sym);
}
/*!
 * Writes in the files M.mat, B.mat and T.mat the block-components of the saddle
 * point matrix
 * @param M The saddle point matrix
 * @return
 */
bool dumpSaddlePointMatrix(const SaddlePointMat &M);

//! Dumps the approximate Schur and approximate inner product matrices, in
//! Shurapprox.mat and Mapprox.mat
/*!
 * @param  M a Saddle Point Matrix
 * @param lumping if true the approximation is built with the lumped version of
 * the inner product matrix. Otherwise, the diagonal part
 */
bool dumpApproximateSchurMatrix(const SaddlePointMat &M, bool lumped);

} // namespace FVCode3D

#endif /* SRC_FVCODE3D_UTILITY_DUMPMATRICES_HPP_ */
