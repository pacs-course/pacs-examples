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
    //! A type dependent on the type of the matrix
    /*
      If the stored matrix is constant this statement define a
      const type for the contained values. I am not using it, but
      is an example of use of conditional.
     */
    using vref_type=
      typename std::conditional<std::is_const<Matrix>::value,
                                const value_type,
                                value_type&>::type;
  public:
    explicit TransposedView(Matrix& A) : ref(A) {}
    

    //! I have to handle the situation when a non const TransposedView stores a const matrix
    /*! 
     The non const version is enabled only if the viewed matrix
      is non-const. I could have used the vref_type defined above
      as return type and I would have obtained my objective.
      But here I show a usage of enable_if
    */
    template<typename T=Matrix>
    std::enable_if_t<!std::is_const<T>::value,value_type &>
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
