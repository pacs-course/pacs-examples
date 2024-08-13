#ifndef HH_TransposeVIEW_HH
#define HH_TransposeVIEW_HH
#include <cmath>
#include <iostream>
#include <type_traits>
#include <typeinfo>
/*! An example on how to build a transpose view
  for a matrix that  works for both cont and non-const matrices

  We only require that the matrix class has an operator
  \f$ (i,j) -> A_{ij} \f$

 */

// namespace to avoid ambiguities with
namespace apsc::LinearAlgebra
{
/*! A trait for the basic types store in the Matrix
  It uses some reasonable default, but it can be specialised
  for your matrix class!.
*/
template <typename T> struct Matrix_Traits
{
  using value_type = double;
  using size_type = std::size_t;
};
//*! A concept to check if a class is a matrix according to my definition
template <typename Matrix> 
concept MatrixConcept = requires (Matrix m)
{
  typename Matrix_Traits<Matrix>::size_type;// The size type must be defined
  typename Matrix_Traits<Matrix>::value_type;// The value type must be defined
  {std::declval<Matrix>()(0, 0)}->std::convertible_to<typename Matrix_Traits<Matrix>::value_type>;// The operator must be defined
};
template <MatrixConcept Matrix> 
class TransposedView
{
public:
  using size_type = typename Matrix_Traits<Matrix>::size_type;
  using value_type = typename Matrix_Traits<Matrix>::value_type;
  //! A more sophisticated method to get value type!
  /*!
   * @code
    using value_type=
    std::remove_reference_t<
      std::remove_cv_t<
        decltype(std::declval<Matrix>()(0,0))
       >
    >;
    @endcode
  */
private:
public:
  TransposedView(Matrix &A) : ref(A) {}

  //! I have to handle the situation when a non const TransposedView stores a
  //! const matrix
  /*!
   The non const version is enabled only if the viewed matrix
    is non-const.
    Uncomment if you want this version of the solution to the problem.
  */
  template <typename T = Matrix>
  std::enable_if_t<!std::is_const_v<T>, value_type &>
  operator()(size_type r, size_type c)
  {
    return ref(c, r);
  }

  /*!
   * A possible alternative is to have the non const operator return
   * a const reference if the stored matrix is const. Uncomment it if you want this version.
   * (however I think the previous solution is cleaner)
   */
  /*
  std::conditional_t<std::is_const_v<Matrix>, const value_type &, value_type &>
  operator()(size_type r, size_type c)
  {
    return ref(c, r);
  }
   */
  /*! 
   * This is the const version. The only available on a const view.
   */
  const value_type
  operator()(size_type r, size_type c) const
  {
    return ref(c, r);
  }

private:
  Matrix &ref;
};
/*!
 This is a deduction guide. It allows to have the correct constructor if you pass a const matrix without specifying the
 template argument!
@code
const Matrix M;
...
TransposedView tr{M}; // generates a TransposedView<const Matrix> !
@endcode
*/
template <typename Matrix>
TransposedView(const Matrix &) -> TransposedView<const Matrix>;

template <typename Matrix> TransposedView(Matrix &) -> TransposedView<Matrix>;

/*!
 * To create a transpose view it is better to use this helper function.
 * It will create the class with the correct "const compliant" template argument
 * @tparam Matrix The Matrix type (automatically deduced)
 * @param A The matrix of which we want to have the transpose view
 * @return The transposed view
 */
template <typename Matrix>
TransposedView<Matrix> inline make_transposeView(Matrix &A)
{
  return TransposedView<Matrix>{A};
}
} // namespace apsc::LinearAlgebra

#endif
