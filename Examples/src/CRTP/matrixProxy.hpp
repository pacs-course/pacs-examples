#ifndef HH_MATRIXPROXY_HH
#define HH_MATRIXPROXY_HH
#include <cstddef>
// A much more complex example of CRTP
// Suppose want to create different matrix classes, each of them
// providing the operator ()(i,j) to address the element in
// row i and column j. This is fine, but I would like also to be
// able to do the same with m[i][j] syntax, like in a "C-style" matrix
// defined as double **. Moreover the matrix may store any type of
// elements, not just double.
// This is a nice trik (from Discovering Modern C++).

namespace apsc::crpt
{
/*!
 * A proxy that stores the information to extract the row.
 * operator[](int i) returns the i-th element of the row
 * @tparam Matrix The matrix type
 * @tparam Result  The returned type
 * @note I could avoid the result type by using automatic return. Or a trait.
 * However, I wanted to sow that a class used for crpt may have more than one
 * template parameters.
 */
template <typename Matrix, typename Result> class bracket_proxy
{
public:
  //! The constructor takes the matrix and the row
  bracket_proxy(Matrix &A, std::size_t r) : A(A), r(r){};
  //! Returns a reference to the matrix  element giving the column index
  Result &
  operator[](std::size_t c)
  {
    return A(r, c);
  }
  //! Returns the element giving the column index
  Result
  operator[](std::size_t c) const
  {
    return A(r, c);
  }

private:
  // A reference to the matrix!
  Matrix &A;
  //! the row index
  std::size_t r;
};

/*
 * The prototype of a matrix with [][] operator
 *
 * @tparam Matrix The matrix type
 * @tparame Result. The result type.
 * */
template <typename Matrix, typename Result> class crtp_matrix
{
  // the proxy for the row
  using const_proxy = bracket_proxy<const Matrix, Result>;
  // the proxy for the roe (constant version)
  using proxy = bracket_proxy<Matrix, Result>;

public:
  //! Returns a proxy to the row
  /*!
   *
   * @param r The row index
   * @return  A proxy to the raw
   */
  proxy
  operator[](std::size_t r)
  {
    // I use list initialization syntax
    // If you prefer, or find less cryptic:
    // return proxy{static_cast<Matrix &>(*this),r};
    //
    return {static_cast<Matrix &>(*this), r};
  }

  /*!
   *  Returns the proxy for the row, but for a const Matrix!
   * @param r The row index
   * @return The proxy holding th einfo on the row (read only version)
   */
  const_proxy
  operator[](std::size_t r) const
  {
    // I use brace initialization...
    return {static_cast<const Matrix &>(*this), r};
  }
};
}
#endif
