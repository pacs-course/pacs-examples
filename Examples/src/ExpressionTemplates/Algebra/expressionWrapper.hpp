#ifndef HH_EXPRESSIONWRAPPER_HH
#define HH_EXPRESSIONWRAPPER_HH
#include <type_traits>
#include <utility>
namespace ET
{
//! A wrapper for expressions
/*!
   This class is an example of use of CRTP (curiosly recursive
   template pattern). Indeed any class that encapsulates an
   expression should derive from it using

   \code
   class AnyExpression: public Expr<AnyExpression>
   {...};
   \endcode

   The derived class should contain the address operator [] and the
   size() method


   The cast operator (also called conversion operator) enables the
   implementation of static polymorphism in a easier way. However, it is not
   strictly necessary: an alternative is to use a method.

 */
template <class E> struct Expr
{
  //! Cast operator to derived class (const version)
  /*!
    Remember that this class is meant to be used with CRTP technique.
    So E is derived from Expr<E>!
    It is never instantiated alone.
    So "this" is always a pointer to an object of the derived class (which is
    E!)
    -> *this is convertible to a reference to E!.
  */
  operator const E &() const { return static_cast<const E &>(*this); }
  //! Cast operator to derived class (non const version)
  operator E &() { return static_cast<E &>(*this); }
  /*! The alternative with a method instead of a cast operator.
     It is less flexible however, but maybe clearer!
  */
  const E &
  asDerived() const
  {
    return static_cast<const E &>(*this);
  }
  E &
  asDerived()
  {
    return static_cast<E &>(*this);
  }

  //! Interrogates the size of the wrapped expression
  std::size_t
  size() const
  {
    return asDerived().size();
  }

  //! Delegates to the wrapped expression the addressing operator
  double
  operator[](std::size_t i) const
  {
    return asDerived().operator[](i);
  }

  //! Delegates to the wrapped expression the addressing operator (non const)
  double &
  operator[](std::size_t i)
  {
    return asDerived().operator[](i);
  }
};
} // namespace ET
#endif
