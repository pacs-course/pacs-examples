#ifndef HH_EXPRESSIONWRAPPER_HH
#define HH_EXPRESSIONWRAPPER_HH
#include <utility>
namespace ET
{
  //! A wrapper for expressions
  /*! 
     This class is an example of use of CPRT (curiosly recursive template pattern). Indeed
     any class that encapsulates an expression should derive from it using 
     class AnyExpression: public Expr<AnyExpression>
     
     The cast operator enable the use of static polymorphism in a easy way.

   */
  template <class E>
  struct Expr
  {
    //! Cast operator to derived class (const version)
    operator const E & () const 
    {
      return static_cast<const E &>(*this);
    }
    //! Cast operator to derived class
    operator E  &() 
    {
      return static_cast<E &>(*this);
    }
    std::size_t size() const {return static_cast<const E &>(*this).size();}
  };
}
#endif
