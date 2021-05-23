/*
 * SparseBlockMatrix.hpp
 *
 *  Created on: Jan 1, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_
#include "Eigen/Sparse"
#include "Eigen/Dense"
#include <array>
#include <memory>
#include <numeric>
#include <exception>
#include <utility>
namespace apsc
{
  /*!
   * Class for setting a block matrix whose blocks are spaerse Matrices
   * When created you have a zero sparse matrix, i.e. a block matrix whose blocks are
   * zero sparse matrices with the specified dimensions
   * @note This class has no default constructor, for safaty reasons. You avoid dangling pointers.
   * @tparam T The stored type
   * @tparam M The number of block rows
   * @tparam N The number of block columns
   * @tparam storageOrder The type of ordiering for the stored Eigen matrices
   *
   * @note So far the class is not default-construcible, far safety reason.
   * To make it default constructible we need some care.
   * It is however copy constructible/assignable and move-constructible/assignable.
   * we
   */
template<typename T, unsigned int M, unsigned int N, int storageOrder=Eigen::ColMajor>
  class SparseBlockMatrix
  {
  public:
  //! The type of the stored Eigen Sparse matrix
  //! @todo this part should go in a trait!
  using SpMat=Eigen::SparseMatrix<T,storageOrder>;
  //! The type of a column vector
  //! @todo this part should go in a trait!
  using ColVector=Eigen::Matrix<T,Eigen::Dynamic,1>;
  //! The type of a row vector
   //! @todo this part should go in a trait!
  using RowVector=Eigen::Matrix<T,1,Eigen::Dynamic>;
  //! The type used to indexing Eigen Matrices
  //! @todo it may be changed by adding a further template argument to SpMat definition of in the trait
  using Index=Eigen::Index;
  //! A structure that holds the indexes of a block. For simplicity I gather row and column index in a struct.
  //! You may use the aggregate initialization to specify a block as {i,j}
  struct Indexes
  {
    Index row;
    Index col;
  };
  /*!
   * The constructor. It takes in input two arrays containing the size of the block rows and columns.
   * The block {i,j} will store a zero matrix with rowSizes[i] rows and colSizes[j] comlumns.
   *
   * @param rowSizes The rows of each block row.
   * @param colSizes The columens of each block column.
   */
  SparseBlockMatrix(std::array<Index, M> rowSizes, std::array<Index, N> colSizes);
  SparseBlockMatrix();

  /*!
   *  Set the block by copying or moving a given matrix
   *  This is not the only way to assign a matrix to a block. An alternative is to use the
   *  getBlock() member function, that returns a reference to the stored matrix block.
   *
   *  setBlock() has more internal tests, while getBlock() has no tests, for efficiency reasons.
   * @tparam Mat Any type convertible to SpMat
   * @param blockPosition The block in the block matrix.
   * @param matrix The matrix to be stored in the block
   */
  template <class Mat>
  void setBlock(Indexes const & blockPosition, Mat&& matrix);

  /*!
   * A block may be the transpose of another block. In this case we just store the pointer to the
   * original block. So when extracting a block {i,j} transpose to a block {k,l} you actually get block {k,l}!
   * To see if it is transposed use isTranspose().
   *
   * @param blockPosition The block which is the transpose of another block
   * @param toBlock The block of which it it the transpose
   */
  void addTranspose(Indexes const & blockPosition, Indexes toBlock);

  /*!
   * Clears the block, leaving dimensions unchanged!
   * @param block
   */
  void clearBlock(Indexes block)
  {
    auto const & [i,j] = block;
    theMatrices[i][j]=std::make_shared<SpMat>( theRowSizes[i], theColSizes[j] );
    transpose[i][j]=false;
  }
  /*!
   * Gets the block. To check if the actual block is the transpose use isTranspose().
   *
   * @param theBlock The block in the form {i,j}
   * @return The block matrix
   */
  SpMat const & getBlock(Indexes const & theBlock) const
  {
    return *(theMatrices[theBlock.row][theBlock.col]);
  }


  /*!
   * Gets the block, const version.
   * @param theBlock The block
   * @return the block matrix
   */
  SpMat  & getBlock(Indexes const & theBlock)
  {
      return *(theMatrices[theBlock.row][theBlock.col]);
  }

  /*!
   * @return The total number of rows of the block matrix
   */
  Index rows()const {return totalRows;}
  /*!
   * @param indexes The block indexes in the form {i,j}
   * @return the number of rows of the block.
   */
  Index rows (Indexes const & indexes)const {return this->theRowSizes[indexes.row];}
  /*!
   * @return The total number of columnss of the block matrix
   */
  Index cols()const {return totalCols;}
  /*!
   * @param indexes The block indexes in the form {i,j}
   * @return the number of columns of the block.
   */
  Index cols (Indexes const & indexes)const {return this->theColSizes[indexes.col];}
  /*!
   * @return true if the matrix is the zero matrix
   */
  bool isEmpty() const {return (totalRows==0 || totalCols==0);}
/*!
 * Reserves space for a block
 * @param block The block indexes
 * @param nonZeroes the nonZeroes expected for the block
 */
  void reserve(Indexes const & block, Index nonZeroes){this->getBlock(block).reserve(nonZeroes);}
  /*!
   * Resizes the matrices to the sizes indicated.
   *
   * Normally called after a default construction of the block matrix.
   * Existing matrices are initialized to zero. Memory is freed.
   * Transpose information is eliminated (set to false). So if you have
   * transposed blocks you have to call addTranspose again.
   * @param rowSizes Array of row block size
   * @param colSizes Array of col block size
   */
  void resize(std::array<Index, M> rowSizes, std::array<Index, N> colSizes);

  /*!
   * Changes the block dimensions and offsets without touching the matrix.
   *
   * Use only if you know wwhat you are doing!.
   * @param rowSizes Array of row block size
   * @param colSizesArray of col block size
   */
  void changeOffsets(std::array<Index, M> rowSizes, std::array<Index, N> colSizes);


  /*!
 * The offset of the block rows in the global matrix
 * @param i The block row index
 * @return The offset
 */
  auto rowOffset(Index i)const {return theRowOffset[i];}
  /*!
   * The offset of the block columss in the global matrix
   * @param i The block column index
   * @return The offset
   */
  auto colOffset(Index j)const {return theColOffset[j];}
  /*!
   * To enquire if the block is in fact the transpose of an actual block.
   * @param block The block indexes {i,j}
   * @return true if it is the transpose
   */
  bool isTranspose(Indexes const & block)const { return transpose[block.row][block.col];}
  /*!
   * @return The number of non zero elements in the block matrix.
   */
  Index nonZeros() const;
  /*!
   * A very expensive method that allows to get the full sparse matrix constructed from the blocks
   * @return The full matrix
   */
  SpMat fullMatrix() const;

  //! Squared Frobenius norm
  double squaredNorm()const
  {
    double res{0.0};
    for (unsigned int i=0; i<M; ++i)
        for (unsigned int j=0; j<N; ++j)
            res+=this->getBlock({i,j}).squaredNorm();
    return res;
  }
  //! Frobenius norm
  double norm() const
  {
    return std::sqrt(this->squaredNorm());
  }

  void makeCompressed()
    {
      for (unsigned int i=0; i<M; ++i)
          for (unsigned int j=0; j<N; ++j)
            if(!this->isTranspose({i,j}))
              this->getBlock({i,j}).makeCompressed();
    }
  //! Clears memory
void clear()
{
  this->resize({0,0},{0,0});
}
 private:
  //! The container of the pointers to the block matrices
  std::array<std::array<std::shared_ptr<SpMat>,M>,N> theMatrices;
  //! The structure that holds the information on transposition
  std::array<std::array<bool,M>,N> transpose;
  //! The row dimension of each block row
  std::array<Index, M> theRowSizes;
  //! The column dimension of each block column
  std::array<Index, N> theColSizes;
  //! The total number of rows
  Index totalRows=0;
  //! The total number of columns
  Index totalCols=0;
  //! The offsets
  std::array<Index,M> theRowOffset;
  std::array<Index,N> theColOffset;
 };

// ****************  OPERATORS  *******************
/*!
 *  Mutiplication of a block matrix and a vector
 * @param A The matrix
 * @param x The vector
 * @return The result of A*x (a column vector)
 */
template <typename T, unsigned int M, unsigned int N,
             int storageOrder>
auto
operator*(SparseBlockMatrix<T, M, N, storageOrder> const & A,
          typename SparseBlockMatrix<T, M, N, storageOrder>::ColVector const & x)
{
  using ColVector=typename SparseBlockMatrix<T, M, N, storageOrder>::ColVector;
  ColVector res=ColVector::Zero(A.rows());

  for (unsigned int i=0; i<M; ++i)
    {
      for (unsigned int j=0; j<N; ++j)
        {
          auto ncols=A.cols({i,j});
          auto nrows=A.rows({i,j});
          if(A.isTranspose({i,j}))
            {
              res.segment(A.rowOffset(i),nrows)+=
                  A.getBlock({i,j}).transpose()*x.segment(A.colOffset(j),ncols);
            }
          else
            {
              res.segment(A.rowOffset(i),nrows)+=
                    A.getBlock({i,j})*x.segment(A.colOffset(j),ncols);
            }
        }
    }
  return res;
}

/*!
 *  Mutiplication of a vector and a block matrix
 * @param A The matrix
 * @param x The vector
 * @return The result of x*A (a row vector!)
 */
template <typename T, unsigned int M, unsigned int N,
             int storageOrder>
auto
operator*(typename SparseBlockMatrix<T, M, N, storageOrder>::RowVector const & x,
          SparseBlockMatrix<T, M, N, storageOrder> const & A
          )
{
  using RowVector=typename SparseBlockMatrix<T, M, N, storageOrder>::RowVector;
  RowVector res;
  res=RowVector::Zero(A.cols());
  for (unsigned int i=0; i<M; ++i)
    {
      for (unsigned int j=0; j<N; ++j)
        {
          auto ncols=A.cols({i,j});
          auto nrows=A.rows({i,j});
          if(A.isTranspose({i,j}))
            {
              res.segment(A.colOffset(j),ncols)+=
                  x.segment(A.rowOffset(i),nrows)* A.getBlock({i,j}).transpose();
            }
          else
            {
              res.segment(A.colOffset(j),ncols)+=
                  x.segment(A.rowOffset(i),nrows)*A.getBlock({i,j});
            }
        }
    }
  return res;
}


//      **********************     IMPLEMENTATIONS   *******************************************


template <typename T, unsigned int M, unsigned int N,
           int storageOrder>
 inline typename SparseBlockMatrix<T, M, N, storageOrder>::SpMat
 SparseBlockMatrix<T, M, N, storageOrder>::fullMatrix() const
 {
  Index nrows = this->totalRows;
  Index ncols = this->totalCols;
  SpMat result(nrows,ncols);
  Index nnz = this->nonZeros();
  result.reserve(nnz);
  using Tri=Eigen::Triplet<T,Index>;
  std::vector<Tri> tripletList;
  for (Index i=0; i<M;++i)
     {
       for (Index j=0; j<N;++j)
         {
           auto rowoff=this->theRowOffset[i];
           auto coloff=this->theColOffset[j];
           SpMat & block= *(theMatrices[i][j]);
           for (Index k=0;k<block.outerSize();++k)
             {
               for (typename SpMat::InnerIterator it(block,k);it;++it)
                 {
                   auto localrow = it.row();
                   auto localcol = it.col();
                   auto value    = it.value();
                   if(this->transpose[i][j])
                     {
                       tripletList.emplace_back(Tri{localcol+rowoff,localrow+coloff,value}  );
                     }
                   else
                     {
                       tripletList.emplace_back(Tri{localrow+rowoff,localcol+coloff,value}  );
                     }
                 }
             }
         }
     }
  result.setFromTriplets (tripletList.begin (), tripletList.end ());
  return result;

 }


  template <typename T, unsigned int M, unsigned int N,
            int storageOrder>
  inline void
  SparseBlockMatrix<T, M, N, storageOrder>::addTranspose(const Indexes & blockPosition,
                                                         Indexes        toBlock)
  {
    auto const & row=blockPosition.row;
    auto const & col=blockPosition.col;
     if (row>=M || col >=N)
       {
         throw std::runtime_error("Block size incorrect an addTranspose");
       }
     transpose[row][col]=true;
     theMatrices[row][col]=theMatrices[toBlock.row][toBlock.col];
  }

  namespace internals
  {
    template <class A, class  B>
    std::tuple<Eigen::Index,Eigen::Index> computeTotals(A const & rowSizes, B const & colSizes)
    {
      return std::make_tuple(
          std::accumulate(colSizes.begin(),colSizes.end(),Eigen::Index{0}),
          std::accumulate(rowSizes.begin(),rowSizes.end(),Eigen::Index{0})
      );
    }
  }

  template <typename T, unsigned int M, unsigned int N, int storageOrder>
  inline SparseBlockMatrix<T, M, N, storageOrder>::SparseBlockMatrix(
    std::array<Index, M> rowSizes, std::array<Index, N> colSizes)
  {
    std::tie(totalCols,totalRows)=internals::computeTotals(rowSizes,colSizes);
    theRowSizes=rowSizes;
    theColSizes=colSizes;
    for (Index i=0; i<M;++i)
      {
        for (Index j=0; j<N;++j)
          {
            theMatrices[i][j] = std::make_shared<SpMat>( rowSizes[i], colSizes[j] );
            transpose[i][j]=false;
          }
      }
    // Compute offfsets
    theRowOffset[0]=0;
    for (Index i=1; i<M;++i)theRowOffset[i]=theRowOffset[i-1]+rowSizes[i-1];
    theColOffset[0]=0;
    for (Index j=1; j<N;++j)theColOffset[j]=theColOffset[j-1]+colSizes[j-1];
  }

  template <typename T, unsigned int M, unsigned int N, int storageOrder>
   void SparseBlockMatrix<T, M, N, storageOrder>::resize(
     std::array<Index, M> rowSizes, std::array<Index, N> colSizes)
   {
     std::tie(totalCols,totalRows)=internals::computeTotals(rowSizes,colSizes);
     theRowSizes=rowSizes;
     theColSizes=colSizes;
     for (Index i=0; i<M;++i)
       {
         for (Index j=0; j<N;++j)
           {
             if (transpose[i][j])
                 theMatrices[i][j] = std::make_shared<SpMat>( rowSizes[i], colSizes[j] );
             else
               {
                 theMatrices[i][j]->resize( rowSizes[i], colSizes[j] );
                 theMatrices[i][j]->data().squeeze();
               }
             transpose[i][j]=false;
           }
       }
     // Compute offfsets
     theRowOffset[0]=0;
     for (Index i=1; i<M;++i)theRowOffset[i]=theRowOffset[i-1]+rowSizes[i-1];
     theColOffset[0]=0;
     for (Index j=1; j<N;++j)theColOffset[j]=theColOffset[j-1]+colSizes[j-1];
   }

  template <typename T, unsigned int M, unsigned int N, int storageOrder>
     void SparseBlockMatrix<T, M, N, storageOrder>::changeOffsets(
       std::array<Index, M> rowSizes, std::array<Index, N> colSizes)
     {
       std::tie(totalCols,totalRows)=internals::computeTotals(rowSizes,colSizes);
       theRowSizes=rowSizes;
       theColSizes=colSizes;
       // Compute offfsets
       theRowOffset[0]=0;
       for (Index i=1; i<M;++i)theRowOffset[i]=theRowOffset[i-1]+rowSizes[i-1];
       theColOffset[0]=0;
       for (Index j=1; j<N;++j)theColOffset[j]=theColOffset[j-1]+colSizes[j-1];
     }

  template <typename T, unsigned int M, unsigned int N, int storageOrder>
    inline SparseBlockMatrix<T, M, N, storageOrder>::SparseBlockMatrix()
  {
    totalCols=0u;
    totalRows=0u;
    theRowSizes.fill(0);
    theColSizes.fill(0);
    for (Index i=0; i<M;++i)
      {
        for (Index j=0; j<N;++j)
          {
            theMatrices[i][j] = std::make_shared<SpMat>();
            transpose[i][j]=false;
          }
      }
    theRowOffset.fill(0);
    theColOffset.fill(0);
 }

 template <typename T, unsigned int M, unsigned int N,
              int storageOrder>
 template <class Mat>
  inline void
   SparseBlockMatrix<T, M, N, storageOrder>::setBlock( const Indexes &blockPosition, Mat&& matrix)
   {
        auto const & row=blockPosition.row;
        auto const & col=blockPosition.col;
        if (row>=M || col >=N)
          {
            throw std::runtime_error("Block size incorrect in setBlock");
          }
        *(theMatrices[row][col])=std::forward<Mat>(matrix);
        Index mtest = (theMatrices[row][col])->rows();
        Index ntest = (theMatrices[row][col])->cols();
        if(mtest != theRowSizes[row] || ntest != theColSizes[col])
          {
            throw std::runtime_error("In setBlock: the added matrix has not the right dimensions");
          }
     }

 template <typename T, unsigned int M, unsigned int N,
            int storageOrder>
  inline typename SparseBlockMatrix<T, M, N, storageOrder>::Index
  SparseBlockMatrix<T, M, N, storageOrder>::nonZeros() const
  {
   Index res{0};
   for (Index i=0; i<M;++i)
      {
        for (Index j=0; j<N;++j)
          {
            res+=this->getBlock({i,j}).nonZeros();
          }
      }
   return res;
  }






 } // end namesace apsc



#endif /* EXAMPLES_SRC_LINEARALGEBRA_SPARSEBLOCKMATRIX_SPARSEBLOCKMATRIX_HPP_ \
        */
