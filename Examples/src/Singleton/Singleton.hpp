#ifndef HHH_SINGLETON_HH
#define HHH_SINGLETON_HH
#include <memory>
/*! A stuct that defines the propotype of a singleton
  If you want your calss to become a singleton do

  class MyClass : public SingletonPrototype<MyClass>

  (another example of CRTP)
*/

template <typename D>
struct SingletonPrototype
{
  SingletonPrototype<D>(SingletonPrototype<D> const &)=delete;
  SingletonPrototype<D>(SingletonPrototype<D> &&)=default;
  SingletonPrototype<D> & operator = (SingletonPrototype<D> const &)=delete;
  SingletonPrototype<D> & operator = (SingletonPrototype<D>&&)=default;
  //! 
  template<typename... Params>
  static D & Instance(Params&&... params)
  {
    //! if not assigned, assign it
    if (! myclass)
      myclass=std::unique_ptr<D>(new D(std::forward<Params>(params)...));
    
    return *myclass;
  }  
protected:
// This activates the sigleton magic.
SingletonPrototype<D>()=default;
static std::unique_ptr<D> myclass;
};

template <typename D>
std::unique_ptr<D> SingletonPrototype<D>::myclass;

#endif

