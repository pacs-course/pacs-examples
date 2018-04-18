#ifndef HH_CLONABLE_HH
#define HH_CLONABLE_HH
#include <type_traits>
#include <memory>
#include <cassert>
/*! Implements the clone() method using the CRTP technique

  To be used you have to make your class inherit publicly from
  clonable<BaseClass,ConcreteClass> where BaseClass is the base class of your
  polymorhic hierarchy of classes. And the ConcreteClass is just yourself...
  See the example in test_clonable.
*/
template <typename B,typename D>
struct clonable
{
  //! no args (maybe not needed
  std::unique_ptr<B> clone()
  {
    static_assert(std::is_base_of<B,D>::value,"template parameter of clonable must be a base class");
    D* asderived = dynamic_cast<D*>(this);
    assert(asderived != nullptr);
    return std::unique_ptr<B>(new D(*asderived));
  }
  virtual ~clonable<B,D>()=default;
};
#endif
