/*
 * SparseBlockMatrix.hpp
 *
 *  Created on: Jan 1, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_
#include "Eigen/Dense"
#include "Eigen/Sparse"
#include <array>
#include <cmath>
#include <exception>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>
namespace apsc
{
/*!
 * Class for building a block matrix whose blocks are sparse matrices.
 * When it is created, it stores a zero block matrix whose blocks are sparse
 * zero matrices with the specified dimensions.
 * @tparam T The stored type
 * @tparam M The number of block rows
 * @tparam N The number of block columns
 * @tparam storageOrder The type of ordering for the stored Eigen matrices
 *
 * @note The class is default constructible. A default-constructed object has
 * zero-sized blocks and can later be initialized with resize().
 * It is also copy constructible/assignable and move constructible/assignable.
 */
template <typename T, unsigned int M, unsigned int N,
          int storageOrder = Eigen::ColMajor>
class SparseBlockMatrix
{
public:
  //! The type of the stored Eigen Sparse matrix
  //! @todo this part should go in a trait!
  using SpMat = Eigen::SparseMatrix<T, storageOrder>;
  //! The type of a column vector
  //! @todo this part should go in a trait!
  using ColVector = Eigen::Matrix<T, Eigen::Dynamic, 1>;
  //! The type of a row vector
  //! @todo this part should go in a trait!
  using RowVector = Eigen::Matrix<T, 1, Eigen::Dynamic>;
  //! The type used to index Eigen matrices.
  //! @todo it may be changed by adding a further template argument to SpMat
  //! definition or in the trait.
  using Index = Eigen::Index;
  //! A structure that stores the indices of a block. You may use aggregate
  //! initialization to specify a block as {i, j}.
  struct Indexes
  {
    Index row;
    Index col;
  };
  /*!
   * The constructor. It takes two arrays containing the sizes of the block
   * rows and columns. Block {i, j} stores a zero matrix with
   * rowSizes[i] rows and colSizes[j] columns.
   *
   * @param rowSizes The size of each block row.
   * @param colSizes The size of each block column.
   */
  SparseBlockMatrix(std::array<Index, M> rowSizes,
                    std::array<Index, N> colSizes);
  SparseBlockMatrix();

  /*!
   * Set a block by copying or moving a matrix.
   * This is not the only way to assign a matrix to a block. An alternative is
   * to use the getBlock() member function, that returns a reference to the
   * stored matrix block.
   *
   * setBlock() performs more checks, while getBlock() performs none for
   * efficiency reasons.
   * @tparam Mat Any type convertible to SpMat
   * @param blockPosition The block in the block matrix.
   * @param matrix The matrix to store in the block.
   */
  template <class Mat>
  void setBlock(Indexes const &blockPosition, Mat &&matrix);

  /*!
   * A block may be the transpose of another block. In this case we store only
   * a pointer to the original block. Therefore, when extracting block {i, j},
   * if it is the transpose of block {k, l}, you actually get block {k, l}.
   * Use isTranspose() to detect this case.
   *
   * @param blockPosition The block that is the transpose of another block.
   * @param toBlock The block of which it is the transpose.
   */
  void addTranspose(Indexes const &blockPosition, Indexes toBlock);

  /*!
   * Clears the block, leaving dimensions unchanged!
   * @param block
   */
  void
  clearBlock(Indexes block)
  {
    auto const &[i, j] = block;
    theMatrices[i][j] = std::make_shared<SpMat>(theRowSizes[i], theColSizes[j]);
    transpose[i][j] = false;
  }
  /*!
   * Gets the block. To check if the actual block is the transpose use
   * isTranspose().
   *
   * @param theBlock The block in the form {i,j}
   * @return The block matrix
   */
  SpMat const &
  getBlock(Indexes const &theBlock) const
  {
    return *(theMatrices[theBlock.row][theBlock.col]);
  }

  /*!
   * Gets the block, non-const version.
   * @param theBlock The block
   * @return the block matrix
   */
  SpMat &
  getBlock(Indexes const &theBlock)
  {
    return *(theMatrices[theBlock.row][theBlock.col]);
  }

  /*!
   * @return The total number of rows of the block matrix
   */
  Index
  rows() const
  {
    return totalRows;
  }
  /*!
   * @param indexes The block indexes in the form {i,j}
   * @return the number of rows of the block.
   */
  Index
  rows(Indexes const &indexes) const
  {
    return this->theRowSizes[indexes.row];
  }
  /*!
   * @return The total number of columns of the block matrix.
   */
  Index
  cols() const
  {
    return totalCols;
  }
  /*!
   * @param indexes The block indexes in the form {i,j}
   * @return the number of columns of the block.
   */
  Index
  cols(Indexes const &indexes) const
  {
    return this->theColSizes[indexes.col];
  }
  /*!
   * @return True if the matrix has zero rows or zero columns.
   */
  bool
  isEmpty() const
  {
    return (totalRows == 0 || totalCols == 0);
  }
  /*!
   * Reserves storage for a block.
   * @param block The block indexes
   * @param nonZeroes The expected number of nonzeros in the block.
   */
  void
  reserve(Indexes const &block, Index nonZeroes)
  {
    this->getBlock(block).reserve(nonZeroes);
  }
  /*!
   * Resizes the matrices to the specified sizes.
   *
   * Normally called after a default construction of the block matrix.
   * Existing matrices are reset to zero. Memory is released.
   * Transpose information is eliminated (set to false). So if you have
   * transposed blocks, you have to call addTranspose() again.
   * @param rowSizes Array with the block-row sizes.
   * @param colSizes Array with the block-column sizes.
   */
  void resize(std::array<Index, M> rowSizes, std::array<Index, N> colSizes);

  /*!
   * Changes the block dimensions and offsets without touching the matrix.
   *
   * Use only if you know what you are doing.
   * @param rowSizes Array with the block-row sizes.
   * @param colSizes Array with the block-column sizes.
   */
  void changeOffsets(std::array<Index, M> rowSizes,
                     std::array<Index, N> colSizes);

  /*!
   * The offset of the block rows in the global matrix
   * @param i The block row index
   * @return The offset
   */
  auto
  rowOffset(Index i) const
  {
    return theRowOffset[i];
  }
  /*!
   * The offset of the block columns in the global matrix
   * @param i The block column index
   * @return The offset
   */
  auto
  colOffset(Index j) const
  {
    return theColOffset[j];
  }
  /*!
   * Checks whether the block is the transpose of another block.
   * @param block The block indexes {i, j}
   * @return True if the block is a transpose view.
   */
  bool
  isTranspose(Indexes const &block) const
  {
    return transpose[block.row][block.col];
  }
  /*!
   * @return The number of nonzero elements in the block matrix.
   */
  Index nonZeros() const;
  /*!
   * A very expensive method that builds the full sparse matrix from the blocks.
   * @return The full matrix.
   */
  SpMat fullMatrix() const;

  //! Squared Frobenius norm
  double
  squaredNorm() const
  {
    double res{0.0};
    for(unsigned int i = 0; i < M; ++i)
      for(unsigned int j = 0; j < N; ++j)
        res += this->getBlock({i, j}).squaredNorm();
    return res;
  }
  //! Frobenius norm
  double
  norm() const
  {
    return std::sqrt(this->squaredNorm());
  }

  void
  makeCompressed()
  {
    for(unsigned int i = 0; i < M; ++i)
      for(unsigned int j = 0; j < N; ++j)
        if(!this->isTranspose({i, j}))
          this->getBlock({i, j}).makeCompressed();
  }
  //! Clears memory
  void
  clear()
  {
    std::array<Index, M> rowSizes{};
    std::array<Index, N> colSizes{};
    this->resize(rowSizes, colSizes);
  }

private:
  //! Container of pointers to the block matrices.
  std::array<std::array<std::shared_ptr<SpMat>, N>, M> theMatrices;
  //! Stores whether a block is represented as a transpose view.
  std::array<std::array<bool, N>, M> transpose;
  //! The row dimension of each block row
  std::array<Index, M> theRowSizes;
  //! The column dimension of each block column
  std::array<Index, N> theColSizes;
  //! The total number of rows
  Index totalRows = 0;
  //! The total number of columns
  Index totalCols = 0;
  //! The offsets
  std::array<Index, M> theRowOffset;
  std::array<Index, N> theColOffset;
};

// ****************  OPERATORS  *******************
/*!
 * Multiplication of a block matrix by a vector.
 * @param A The matrix
 * @param x The vector
 * @return The result of A*x (a column vector).
 */
template <typename T, unsigned int M, unsigned int N, int storageOrder>
auto
operator*(SparseBlockMatrix<T, M, N, storageOrder> const &                    A,
          typename SparseBlockMatrix<T, M, N, storageOrder>::ColVector const &x)
{
  using ColVector =
    typename SparseBlockMatrix<T, M, N, storageOrder>::ColVector;
  ColVector res = ColVector::Zero(A.rows());

  for(unsigned int i = 0; i < M; ++i)
    {
      for(unsigned int j = 0; j < N; ++j)
        {
          auto ncols = A.cols({i, j});
          auto nrows = A.rows({i, j});
          if(A.isTranspose({i, j}))
            {
              res.segment(A.rowOffset(i), nrows) +=
                A.getBlock({i, j}).transpose() *
                x.segment(A.colOffset(j), ncols);
            }
          else
            {
              res.segment(A.rowOffset(i), nrows) +=
                A.getBlock({i, j}) * x.segment(A.colOffset(j), ncols);
            }
        }
    }
  return res;
}

/*!
 * Multiplication of a vector by a block matrix.
 * @param A The matrix
 * @param x The vector
 * @return The result of x*A (a row vector).
 */
template <typename T, unsigned int M, unsigned int N, int storageOrder>
auto
operator*(typename SparseBlockMatrix<T, M, N, storageOrder>::RowVector const &x,
          SparseBlockMatrix<T, M, N, storageOrder> const &                    A)
{
  using RowVector =
    typename SparseBlockMatrix<T, M, N, storageOrder>::RowVector;
  RowVector res;
  res = RowVector::Zero(A.cols());
  for(unsigned int i = 0; i < M; ++i)
    {
      for(unsigned int j = 0; j < N; ++j)
        {
          auto ncols = A.cols({i, j});
          auto nrows = A.rows({i, j});
          if(A.isTranspose({i, j}))
            {
              res.segment(A.colOffset(j), ncols) +=
                x.segment(A.rowOffset(i), nrows) *
                A.getBlock({i, j}).transpose();
            }
          else
            {
              res.segment(A.colOffset(j), ncols) +=
                x.segment(A.rowOffset(i), nrows) * A.getBlock({i, j});
            }
        }
    }
  return res;
}

//      **********************     IMPLEMENTATIONS
//      *******************************************

template <typename T, unsigned int M, unsigned int N, int storageOrder>
inline typename SparseBlockMatrix<T, M, N, storageOrder>::SpMat
SparseBlockMatrix<T, M, N, storageOrder>::fullMatrix() const
{
  Index nrows = this->totalRows;
  Index ncols = this->totalCols;
  SpMat result(nrows, ncols);
  Index nnz = this->nonZeros();
  result.reserve(nnz);
  using Tri = Eigen::Triplet<T, Index>;
  std::vector<Tri> tripletList;
  for(Index i = 0; i < M; ++i)
    {
      for(Index j = 0; j < N; ++j)
        {
          auto   rowoff = this->theRowOffset[i];
          auto   coloff = this->theColOffset[j];
          SpMat &block = *(theMatrices[i][j]);
          for(Index k = 0; k < block.outerSize(); ++k)
            {
              for(typename SpMat::InnerIterator it(block, k); it; ++it)
                {
                  auto localrow = it.row();
                  auto localcol = it.col();
                  auto value = it.value();
                  if(this->transpose[i][j])
                    {
                      tripletList.emplace_back(
                        Tri{localcol + rowoff, localrow + coloff, value});
                    }
                  else
                    {
                      tripletList.emplace_back(
                        Tri{localrow + rowoff, localcol + coloff, value});
                    }
                }
            }
        }
    }
  result.setFromTriplets(tripletList.begin(), tripletList.end());
  return result;
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
inline void
SparseBlockMatrix<T, M, N, storageOrder>::addTranspose(
  const Indexes &blockPosition, Indexes toBlock)
{
  auto const &row = blockPosition.row;
  auto const &col = blockPosition.col;
  if(row >= M || col >= N || toBlock.row >= M || toBlock.col >= N)
    {
      throw std::runtime_error("Invalid block index in addTranspose");
    }
  if(theRowSizes[row] != theColSizes[toBlock.col] ||
     theColSizes[col] != theRowSizes[toBlock.row])
    {
      throw std::runtime_error(
        "In addTranspose: incompatible block dimensions");
    }
  transpose[row][col] = true;
  theMatrices[row][col] = theMatrices[toBlock.row][toBlock.col];
}

namespace internals
{
  template <class A, class B>
  std::tuple<Eigen::Index, Eigen::Index>
  computeTotals(A const &rowSizes, B const &colSizes)
  {
    return std::make_tuple(
      std::accumulate(colSizes.begin(), colSizes.end(), Eigen::Index{0}),
      std::accumulate(rowSizes.begin(), rowSizes.end(), Eigen::Index{0}));
  }
} // namespace internals

template <typename T, unsigned int M, unsigned int N, int storageOrder>
inline SparseBlockMatrix<T, M, N, storageOrder>::SparseBlockMatrix(
  std::array<Index, M> rowSizes, std::array<Index, N> colSizes)
{
  std::tie(totalCols, totalRows) = internals::computeTotals(rowSizes, colSizes);
  theRowSizes = rowSizes;
  theColSizes = colSizes;
  for(Index i = 0; i < M; ++i)
    {
      for(Index j = 0; j < N; ++j)
        {
          theMatrices[i][j] = std::make_shared<SpMat>(rowSizes[i], colSizes[j]);
          transpose[i][j] = false;
        }
    }
  // Compute offsets.
  if constexpr(M > 0)
    {
      theRowOffset[0] = 0;
      for(Index i = 1; i < M; ++i)
        theRowOffset[i] = theRowOffset[i - 1] + rowSizes[i - 1];
    }
  if constexpr(N > 0)
    {
      theColOffset[0] = 0;
      for(Index j = 1; j < N; ++j)
        theColOffset[j] = theColOffset[j - 1] + colSizes[j - 1];
    }
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
void
SparseBlockMatrix<T, M, N, storageOrder>::resize(std::array<Index, M> rowSizes,
                                                 std::array<Index, N> colSizes)
{
  std::tie(totalCols, totalRows) = internals::computeTotals(rowSizes, colSizes);
  theRowSizes = rowSizes;
  theColSizes = colSizes;
  for(Index i = 0; i < M; ++i)
    {
      for(Index j = 0; j < N; ++j)
        {
          if(transpose[i][j] || !theMatrices[i][j])
            theMatrices[i][j] =
              std::make_shared<SpMat>(rowSizes[i], colSizes[j]);
          else
            {
              theMatrices[i][j]->resize(rowSizes[i], colSizes[j]);
              theMatrices[i][j]->data().squeeze();
            }
          transpose[i][j] = false;
        }
    }
  // Compute offsets.
  if constexpr(M > 0)
    {
      theRowOffset[0] = 0;
      for(Index i = 1; i < M; ++i)
        theRowOffset[i] = theRowOffset[i - 1] + rowSizes[i - 1];
    }
  if constexpr(N > 0)
    {
      theColOffset[0] = 0;
      for(Index j = 1; j < N; ++j)
        theColOffset[j] = theColOffset[j - 1] + colSizes[j - 1];
    }
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
void
SparseBlockMatrix<T, M, N, storageOrder>::changeOffsets(
  std::array<Index, M> rowSizes, std::array<Index, N> colSizes)
{
  std::tie(totalCols, totalRows) = internals::computeTotals(rowSizes, colSizes);
  theRowSizes = rowSizes;
  theColSizes = colSizes;
  // Compute offsets.
  if constexpr(M > 0)
    {
      theRowOffset[0] = 0;
      for(Index i = 1; i < M; ++i)
        theRowOffset[i] = theRowOffset[i - 1] + rowSizes[i - 1];
    }
  if constexpr(N > 0)
    {
      theColOffset[0] = 0;
      for(Index j = 1; j < N; ++j)
        theColOffset[j] = theColOffset[j - 1] + colSizes[j - 1];
    }
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
inline SparseBlockMatrix<T, M, N, storageOrder>::SparseBlockMatrix()
{
  std::array<Index, M> rowSizes{};
  std::array<Index, N> colSizes{};
  this->resize(rowSizes, colSizes);
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
template <class Mat>
inline void
SparseBlockMatrix<T, M, N, storageOrder>::setBlock(const Indexes &blockPosition,
                                                   Mat &&         matrix)
{
  auto const &row = blockPosition.row;
  auto const &col = blockPosition.col;
  if(row >= M || col >= N)
    {
      throw std::runtime_error("Invalid block index in setBlock");
    }
  auto replacement = std::make_shared<SpMat>(std::forward<Mat>(matrix));
  Index mtest = replacement->rows();
  Index ntest = replacement->cols();
  if(mtest != theRowSizes[row] || ntest != theColSizes[col])
    {
      throw std::runtime_error(
        "In setBlock: the added matrix does not have the expected dimensions");
    }
  theMatrices[row][col] = std::move(replacement);
  transpose[row][col] = false;
}

template <typename T, unsigned int M, unsigned int N, int storageOrder>
inline typename SparseBlockMatrix<T, M, N, storageOrder>::Index
SparseBlockMatrix<T, M, N, storageOrder>::nonZeros() const
{
  Index res{0};
  for(Index i = 0; i < M; ++i)
    {
      for(Index j = 0; j < N; ++j)
        {
          res += this->getBlock({i, j}).nonZeros();
        }
    }
  return res;
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_ \
        */
