/*!
 * @file SparseBlock.hpp
 * @brief Extract Sparse Block for SpMat
 */
#ifndef FVCODE3D_SPARSEBLOCK_HPP_
#define FVCODE3D_SPARSEBLOCK_HPP_
#include "BasicType.hpp"
#include <Eigen/Sparse>
#include <exception>
#include <vector>

namespace FVCode3D
{
//! Extracts a block from a Eigen Sparse Matrix
/*!
 *
 * @tparam SparseMatrix The Eigen sparse matrix type
 * @param matrix The matrix from which block is extracted
 * @param startRow Starting row for extraction
 * @param startCol Starting columns for extraction
 * @param nRows Number of rows of extracted matrix
 * @param nCols Number of columns of extracted matrix
 * @return The extracted matrix
 * @throw runtime_exception if block sizing is incorrect
 */
template <class SparseMatrix>
auto
sparseBlock(SparseMatrix const &matrix, UInt startRow, UInt startCol,
            UInt nRows, UInt nCols)
{
  // block for check
  {
    auto rowsOriginal = matrix.rows();
    auto colsOriginal = matrix.cols();
    if(startRow + nRows > rowsOriginal || startCol + nCols > colsOriginal)
      throw std::runtime_error("sparseBlock: sizing of block is inconsistent");
  }

  using Triplet = Eigen::Triplet<typename SparseMatrix::Scalar>;
  std::vector<Triplet> triplets;
  triplets.reserve(matrix.nonZeros());
  for(int k = 0; k < matrix.outerSize(); ++k)
    for(typename SparseMatrix::InnerIterator it(matrix, k); it; ++it)
      {
        auto i = it.row();
        auto j = it.col();
        if(i >= startRow && i < startRow + nRows)
          {
            if(j >= startCol && j < startCol + nCols)
              {
                triplets.emplace_back(i - startRow, j - startCol, it.value());
              }
          }
      }
  SparseMatrix R(nRows, nCols);
  R.setFromTriplets(triplets.begin(), triplets.end());
  return R;
}
} // namespace FVCode3D

#endif // FVCODE3D_SPARSEBLOCK_HPP_
