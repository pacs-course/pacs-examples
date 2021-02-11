#ifndef HH_TransposeVIEW_HH
#define HH_TransposeVIEW_HH
#include <iostream>
#include <cmath>
#include <typeinfo>
#include <type_traits>
/*! An example on how to build a transpose view
  for a matrix that  works for both cont and non-const matrices

  We only require that the matrix class has an operator 
  \f$ (i,j) -> A_{ij} \f$

 */

// namespace to avoid ambiguities with
namespace apsc::LinearAlgebra {

  /*! A trait for the basic types store in the Matrix
    It uses some reasonable default, but it can be specialised
    for your matrix class!.
  */
  template <typename T>
  struct Matrix_Traits
  {
    using value_type=double;
    using size_type=std::size_t;
  };


  template <typename Matrix>
  class TransposedView
  {
  public:
    using size_type=  typename Matrix_Traits<Matrix>::size_type;
    using value_type= typename Matrix_Traits<Matrix>::value_type;
    //! A more sophisticated method to get value type!
    /*!
     * @code
      using value_type=
      typename std::remove_reference<
      typename std::remove_cv<
      decltype(std::declval<Matrix>()(0,0))
      >::type>::type;
      @endcode
    */
  private:

  public:
     TransposedView(Matrix& A) : ref(A) {}
    
    //! I have to handle the situation when a non const TransposedView stores a const matrix
    /*! 
     The non const version is enabled only if the viewed matrix
      is non-const.
    */
    template<typename T=Matrix>
    std::enable_if_t<!std::is_const_v<T>,value_type &>
    operator()(size_type r, size_type c)
    {
      return ref(c, r);
    }
    
    const value_type operator()(size_type r, size_type c) const
    {
      return ref(c, r);
    }
    
  private:
    Matrix& ref;
  };
  /*!
   This is a deduction guide.
   */
  template <typename Matrix>
  TransposedView(const Matrix & )->TransposedView<const Matrix>;
  template <typename Matrix>
  TransposedView(Matrix & )->TransposedView<Matrix>;

  
  /*!
   * To create a transpose view it is better to use this helper function.
   * It will create the class with the correct "const compliant" template argument
   * @tparam Matrix The Matrix type (automatically deduced)
   * @param A The matrix of which we want to have the transpose view
   * @return The transposed view
   */
  template <typename Matrix>
  TransposedView<Matrix> inline make_transposeView(Matrix& A)
  {
    return TransposedView<Matrix>{A};
  }
}// end namespace

#endif
