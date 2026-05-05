#ifndef HH_MYMAT0_Views_HH
#define HH_MYMAT0_Views_HH
#include "MyMat0.hpp"
#include <algorithm>
#include <stdexcept>
#include <type_traits>
namespace LinearAlgebra
{
//! @file MyMat0_views.hpp
/*!
  Example of a view class applied to `MyMat0`.

  A view is an object that reuses an existing resource but exposes a different
  semantic interface. In this file, the transpose of a matrix is represented
  without building a new matrix. The view delegates the work to the underlying
  matrix and remaps the meaning of the public operations.
 */

//! Transpose view of a matrix-like object.
/*!
  The class stores a reference to an existing matrix and exposes the interface
  of its transpose.
  @tparam MAT Underlying matrix type.
 */
template <typename MAT> class TransposeView
{
public:
  using value_type = typename MAT::value_type;
  using size_type = typename MAT::size_type;

private:
public:
  //! Builds a transpose view from an existing matrix.
  /*!
    @param m Matrix to be viewed through transposed semantics.
   */
  explicit TransposeView(MAT &m) : M_mat(m) {}
  //! It will give an error if the viewed matrix is constant
  /*!
    Sets all entries of the underlying matrix to zero through the view.
   */
  void
  fillZero()
  {
    std::fill(M_mat.begin(), M_mat.end(), value_type{});
  }
  //! Resizing the matrix
  /*!
   * I resize the reference, exchanging rows with cols
   * It will give an error if the viewed matrix is constant.
   * @param nrow New number of rows of the view.
   * @param ncol New number of columns of the view.
   */
  void
  resize(size_type const nrow, size_type const ncol)
  {
    M_mat.resize(ncol, nrow);
  }
  //! Number of rows
  size_type
  nrow() const
  {
    return M_mat.ncol();
  }
  //! Number of columns
  size_type
  ncol() const
  {
    return M_mat.nrow();
  }
  //! Returns element with no bound check (non-const version)
  /*!
    The version that returns a reference, and thus allows to modify
    the underline matrix, is activated only if the underlying matrix is not
    const This avoids errors if I call the operator on a transpose view of a
    constant matrix! Note the trick of a default template parameter to allow
    enable_if to work
    @param i Row index in the view.
    @param j Column index in the view.
    @return Reference to the selected entry of the underlying matrix.
   */
  template <typename T = MAT>
  std::enable_if_t<!std::is_const_v<T>, value_type &>
  operator()(const size_type i, const size_type j)
  {
    return M_mat(j, i);
  }
  //! Returns a view entry in a constant context.
  /*!
    @param i Row index in the view.
    @param j Column index in the view.
    @return Value of the selected entry.
   */
  value_type
  operator()(const size_type i, const size_type j) const
  {
    return M_mat(j, i);
  }
  //! Policy cannot be changed, but it may be queried
  constexpr StoragePolicySwitch
  getStoragePolicy() const
  {
    return M_mat.getStoragePolicy();
  }

  //! Computes the infinity norm of the transpose view.
  /*!
    For a transpose, the infinity norm coincides with the 1-norm of the
    underlying matrix.
   */
  auto
  normInf() const
  {
    return M_mat.norm1();
  }

  //! Computes the 1-norm of the transpose view.
  /*!
    For a transpose, the 1-norm coincides with the infinity norm of the
    underlying matrix.
   */
  auto
  norm1() const
  {
    return M_mat.normInf();
  }

  //! Computes the Frobenius norm of the transpose view.
  auto
  normF() const
  {
    return M_mat.normF();
  }
  //! Multiplies the transpose view by a vector.
  /*!
    @param v Right-hand-side vector.
    @param res Vector where the result is stored.
    @throws std::invalid_argument if `v.size()!=ncol()`.
   */
  void vecMultiply(const std::vector<value_type> &v,
                   std::vector<value_type> &      res) const;

  //! Fills the underlying matrix with random values.
  /*!
    @param seed Optional seed forwarded to the underlying matrix.
   */
  auto
  fillRandom(unsigned int seed = 0)
  {
    return M_mat.fillRandom(seed);
  }

  //! Prints the transpose view.
  /*!
    @param out Output stream.
   */
  void showMe(std::ostream &out = std::cout) const;

private:
  MAT &M_mat;
};

template <typename MAT>
void
TransposeView<MAT>::vecMultiply(const std::vector<value_type> &v,
                                std::vector<value_type> &      res) const
{
  if(v.size() != this->ncol())
    throw std::invalid_argument("Vector size must match the number of view columns");
  res.assign(this->nrow(), value_type{});
  // for efficiency I use two different algorithms
  // I could have used overloading!
  switch(M_mat.getStoragePolicy())
    {
    case COLUMNMAJOR:
      // Classic A^T*v row by row
      for(size_type i = 0; i < M_mat.ncol() * M_mat.nrow(); ++i)
        res[i / M_mat.nrow()] += M_mat[i] * v[i % M_mat.nrow()];
      break;
    case ROWMAJOR:
      // result is a linear combination of the columns of A^T
      for(size_type i = 0; i < M_mat.ncol() * M_mat.nrow(); ++i)
        res[i % M_mat.ncol()] += M_mat[i] * v[i / M_mat.ncol()];
      break;
    }
}

template <typename MAT>
void
TransposeView<MAT>::showMe(std::ostream &out) const
{
  auto const nr = this->nrow();
  auto const nc = this->ncol();
  if(nr == 0 || nc == 0)
    {
      out << "[]\n";
      return;
    }
  out << "[";
  for(size_type i = 0; i < nr; ++i)
    {
      for(size_type j = 0; j < nc - 1; ++j)
        out << this->operator()(i, j) << ", ";
      out << this->operator()(i, nc - 1);
      if(i == nr - 1)
        out << "]" << std::endl;
      else
        out << std::endl;
    }
}

template <class T, StoragePolicySwitch storagePolicy>
std::vector<T>
operator*(TransposeView<MyMat0<T, storagePolicy> > const &m,
          std::vector<T> const &                          v)
{
  std::vector<T> tmp;
  m.vecMultiply(v, tmp);
  return tmp;
}

} // namespace LinearAlgebra
#endif
