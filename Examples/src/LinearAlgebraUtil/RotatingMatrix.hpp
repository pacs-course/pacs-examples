/*
 * RotatingMatrix.hpp
 *
 *  Created on: Nov 18, 2018
 *      Author: forma
 */

#ifndef SRC_LINEARALGEBRA_UTILITIES_RotatingMatrix_HPP_
#define SRC_LINEARALGEBRA_UTILITIES_RotatingMatrix_HPP_
#include "Eigen/Dense"
#include <algorithm>
#include <vector>

namespace apsc
{
/*!
 * This enum changes the way a new column is stored in the
 * RotatingMatrix when you exceed the maximum. In particular, if you use
 * NewestReplacesOldest the new column replaces the oldest (faster, but you
 * loose the ordering of the columns). With Shift you shift the columns so that
 * the last inserted is always the last column in the matrix and the order
 * follows the order of insertion.
 */
enum class InsertStrategy
{
  NewestReplacesOldest,
  Shift
};

//! This class represents a matrix with N columns that keeps only the latest
//! inserted columns. It is strongly baded on the eigen
//!
//! @tparam T The type stored in the matrix
//! @tparam M the number of rows
//! @tparam N the number of columns
template <class T, std::size_t M, std::size_t N,
          InsertStrategy Strategy = InsertStrategy::Shift>
class RotatingMatrix
{
public:
  // Needed since I am using a fixed size eigen matric as class member
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Vector = Eigen::Matrix<T, M, 1>;
  using Matrix = Eigen::Matrix<T, M, N, Eigen::ColMajor>;
  //! Returns the underlying container
  //! This way I have access also to all other methods of Eigen
  auto const &
  getEigenFullMatrix() const
  {
    return M_vec;
  }
  //! Returns the underlying container
  auto &
  getEigenFullMatrix()
  {
    return M_vec;
  }
  //! Returns j-th column as an Eigen Vector
  auto
  col(std::size_t j) const
  {
    return M_vec.col(j);
  }
  //! Returns j-th column as an Eigen Vector
  decltype(auto)
  col(std::size_t j)
  {
    return M_vec.col(j);
  }
  //! Returns only the active parts of matrix as an Eigen Matrix Block
  //! @pre I must have inserted at least one column
  decltype(auto)
  getMatrix()
  {
    return M_vec.block(0, 0, M, N_size);
  }
  //! Returns only the active parts of matrix as an Eigen Matrix Block
  //! @pre I must have inserted at least one column
  auto
  getMatrix() const
  {
    return M_vec.block(0, 0, M, N_size);
  }
  //! The current inserted number of columns
  auto
  cols() const
  {
    return N_size;
  }
  //! The max number of columns that may be inserted
  static constexpr auto
  max_size()
  {
    return N;
  }
  //! The max number of columns has been reached
  bool
  full() const
  {
    return N_size == N;
  }
  //! true if empty
  bool
  empty() const
  {
    return N_size == 0u;
  }
  //! Add a new column
  void
  push_back(const Vector &arg)
  {
    if(N > N_size)
      {
        M_vec.col(N_size) = arg;
        ++N_size;
      }
    else
      {
        if constexpr(Strategy == InsertStrategy::Shift)
          {
            for(std::size_t i = 0u; i < N_size - 1u; ++i)
              M_vec.col(i) = M_vec.col(i + 1);
            M_vec.col(N - 1) = arg;
          }
        else
          {
            M_vec.col(oldestCol) = arg;
            oldestCol = (oldestCol + 1u) % N;
          }
      }
  }
  //! reset the valid column counter
  void
  reset()
  {
    N_size = 0u;
  }

private:
  std::size_t N_size = 0u;
  std::size_t oldestCol = 0u;
  Matrix      M_vec;
};

//! A variant of RotatingMatrix<T,N> where the number of rows and
//! maximal number of columns are set at run time
/*!
 *  Useful if you need to keep track of just the last inserted
 *  elements, up to a maximal number. When the max size is reached,
 *  a newly inserted element will cause the elimination of the front element,
 *  a shift to the left by one, and the addition at the back.
 *  It is a thin wrapper around an std::vector, where I redefine
 *  emplace_back and push_back, and back methods.
 *  The vector is composed in the class.
    /tparam T the type stored in the rotating vector
 */

template <class T, InsertStrategy Strategy = InsertStrategy::Shift>
class RotatingMatrixX
{
public:
  using Vector = Eigen::Matrix<T, Eigen::Dynamic, 1>;
  using Matrix =
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;
  RotatingMatrixX() = default;
  RotatingMatrixX(size_t nrows, size_t ncols)
  {
    M_vec.resize(nrows, ncols);
    M = nrows;
    N = ncols;
  }
  //! Returns the underlying container
  //! This way I have access also to all other methods of Eigen
  auto const &
  getEigenFullMatrix() const
  {
    return M_vec;
  }
  //! Returns the underlying container
  auto &
  getEigenFullMatrix()
  {
    return M_vec;
  }
  //! Returns j-th column as an Eigen Vector
  auto
  col(std::size_t j) const
  {
    return M_vec.col(j);
  }
  //! Returns j-th column as an Eigen Vector
  decltype(auto)
  col(std::size_t j)
  {
    return M_vec.col(j);
  }
  //! Returns only the active parts of matrix as an Eigen Matrix Block
  //! @pre I must have inserted at least one column
  decltype(auto)
  getMatrix()
  {
    return M_vec.block(0, 0, M, N_size);
  }
  //! Returns only the active parts of matrix as an Eigen Matrix Block
  //! @pre I must have inserted at least one column
  auto
  getMatrix() const
  {
    return M_vec.block(0, 0, M, N_size);
  }
  //! The current inserted number of columns
  auto
  cols() const
  {
    return N_size;
  }
  //! The max number of columns that may be inserted
  auto
  max_size()
  {
    return N;
  }
  //! The max number of columns has been reached
  bool
  full() const
  {
    return N_size == N;
  }
  //! true if empty
  bool
  empty() const
  {
    return N_size == 0u;
  }
  //! Add a new comlumn
  void
  push_back(const Vector &arg)
  {
    if(N > N_size)
      {
        M_vec.col(N_size) = arg;
        ++N_size;
      }
    else
      {
        if constexpr(Strategy == InsertStrategy::Shift)
          {
            for(std::size_t i = 0u; i < N_size - 1u; ++i)
              M_vec.col(i) = M_vec.col(i + 1);
            M_vec.col(N - 1) = arg;
          }
        else
          {
            M_vec.col(oldestCol) = arg;
            oldestCol = (oldestCol + 1u) % N;
          }
      }
  }
  void
  reset()
  {
    N_size = 0;
  }

private:
  size_t      N{0u};
  size_t      M{0u};
  std::size_t N_size = 0u;
  Matrix      M_vec;
  size_t      oldestCol = 0u;
};

} // end namespace apsc

#endif /* SRC_LINEARALGEBRA_UTILITIES_RotatingMatrix_HPP_ */
