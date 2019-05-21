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
namespace LinearAlgebra {

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
    /*
      using value_type=
      typename std::remove_reference<
      typename std::remove_cv<
      decltype(std::declval<Matrix>()(0,0))
      >::type>::type;
    */
  private:
    //! I have to handle the situation when a non const TransposedView stores a const matrix
    //!
    //! In this case also the non-const version of the call operator
    //! used to fetch the Matrix element returns a const value. So it cannot
    //! be used in a rvalue context. Another possibility is to use
    //! enable_if and simply delete the non const version in the case of const Matrix

    using vref_type=
      typename std::conditional<std::is_const<Matrix>::value,
                                const value_type,
                                value_type&>::type;
  public:
    explicit TransposedView(Matrix& A) : ref(A) {}
    
    /*! The non const version is enabled only if the viewed matrix
      is non-const
    */
    vref_type
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
  
  //! This returns the transposed view of a matrix
  template <typename Matrix>
  TransposedView<Matrix> inline trans(Matrix& A)
  {
    return TransposedView<Matrix>(A);
  }
}// end namespace

#endif
