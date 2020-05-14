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
   *  @param sym true if the matrix is symmetric. The matrix market file will store only the lower dimensional part
   *  @return false if something wrong happened
   */
  template <typename SparseMat>
  bool dumpSparseMatrix(const SparseMat & M, const std::string& filename, bool sym=false)
  {
    return Eigen::saveMarket(M,filename,sym);
  }
  //! Dumps all components of a SaddlePointMat
   bool dumpSaddlePointMatrix(const SaddlePointMat & M);

   //! Dumps approximate Schur and approximate inner product matrices
   /*!
    * @param  M a Saddle Point Matrix
    * @param lumping if true the approximation is built with the lumped version of the
    *        inner product matrix. Otherwise, the diagonal
    */
    bool dumpApproximateSchurMatrix(const SaddlePointMat & M, bool lumped);


}


#endif /* SRC_FVCODE3D_UTILITY_DUMPMATRICES_HPP_ */
