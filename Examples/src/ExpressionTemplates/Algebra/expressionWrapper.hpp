#ifndef HH_EXPRESSIONWRAPPER_HH
#define HH_EXPRESSIONWRAPPER_HH
#include <utility>
namespace ET
{
  template <class E>
  struct Expr
  {
    //! Cast operator to derived class
    operator const E & () const 
    {
      return static_cast<const E &>(*this);
    }
    //! Cast operator to derived class
    operator E  &() 
    {
      return static_cast<E &>(*this);
    }
    std::size_t size()const {return  static_cast<const E &>(*this).size();}
  };
}
#endif
