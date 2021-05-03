#include "CloningUtilities.hpp"
#include <iostream>
#include <iomanip>
#include <type_traits>
struct C
{
  C * clone(){return new C(*this);}
  int i;
};

struct NC
{
  int i;
};

template <class T>
class NiftyContainer{
public:
  void DoSomething(T* pObj)
  {
    using namespace apsc::TypeTraits;
    if constexpr (isClonable<T>())
    {
      T* pNewObj = pObj->clone();
      pNewObj->i=10; // avoid warning for unused variable
      std::cout<<"Using polymorphic algorithm"<<std::endl;
    }
    else
      {
	T* pNewObj = new T(*pObj);
	std::cout<<"Using non polymorphic algorithm"<<std::endl;
	delete pNewObj;
      }
  }
};


int main()
{
  using namespace apsc::TypeTraits;
  std::cout<<"C is clonable:  "<<std::boolalpha<< isClonable<C>()<<std::endl;
  NiftyContainer<C> ncc;
  C pc;
  ncc.DoSomething(&pc);
  std::cout<<"NC is clonable: "<<std::boolalpha<< isClonable<NC>()<<std::endl;
  NiftyContainer<NC> ncnc;
  NC pnc;
  ncnc.DoSomething(&pnc);
}

