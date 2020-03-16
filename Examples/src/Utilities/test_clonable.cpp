// a test for cloning utilitiess
#include "CloningUtilities.hpp"
#include <iostream>

//A nonclonable class
class NoClone{
public:
  int a;
};

//A hierarchy of clonable classes
//! the base class
/*!
  I want the hyearchy to be clonable so I set clone() method.
 */
class Base
{
public:
  virtual void whoAmI()=0;
  virtual void setValue(double const &)=0;
  virtual std::unique_ptr<Base> clone()=0;
  virtual ~Base()=default;
};

//! Derived class
class Derived1: public Base
{
public:
  void setValue(double const & inp)override {value_=inp;}
  void whoAmI() override
  {
    std::cout<<" I am Derived1 and I store "<<value_<<std::endl;
  }
  std::unique_ptr<Base> clone() override
  {
    return std::make_unique<Derived1>(*this);
  };
private:
  double value_=1.0;
};

class Derived2: public Base
{
public:
  void setValue(double const & inp)override {value_=inp;}
  void whoAmI() override
  {
    std::cout<<" I am Derived2 and I store "<<value_<<std::endl;
  }
  std::unique_ptr<Base> clone() override
  {
    return std::make_unique<Derived2>(*this);
  };
private:
  double value_=2.0;
};

//! Now the container class
/*!  
  We want to compose in the container class an element of the Base
  hierarchy. We want to be able to define at run time the concrete
  type of the contained object and implement composition, i.e. the
  container has full ownership of the resource and it implements deep
  copy in the case it is copied! And without the need of special
  construtor or assignment operators. This is possible thanks to the
  clone technique (also called virtual construtor)
 */
class Container
{
public:
  //! A very simple object factory!
  /*
    /param[in] i if i=1 the resource is of type Derived1, otherwise Derived2
   */
  void chooseResource(int i)
  {
    if(i==1)
      {
        resource_=apsc::make_Wrapper<Base,Derived1>();
      }else
      {
        resource_=apsc::make_Wrapper<Base,Derived2>();
      }
  }
  // ! I can access the resource by reference
  Base & resource(){return *resource_;}
  Base const & resource()const {return *resource_;}
private:
  apsc::Wrapper<Base> resource_;
};

int main()
{
  //! Testing has_clone
  std::cout<<std::boolalpha;
  std::cout<<"In NoClone clonable? "<<apsc::has_clone<NoClone>()<<std::endl;
  std::cout<<"In Derived2 clonable? "<<apsc::has_clone<Derived2>()<<std::endl;
  std::cout<<std::endl;
  Container A;
  A.chooseResource(1);
  A.resource().setValue(99.);
  std::cout<<"The resource in A is: ";
  A.resource().whoAmI();
  Container B{A};// B contains A COPY of the resource of A
  std::cout<<"The resource in B is: ";
  B.resource().whoAmI();
  // I can change the value
  B.resource().setValue(55.);
  std::cout<<"Now the resource in B is: ";
  B.resource().whoAmI();
  // But in A is unchenged, since A own its own resource uniquely (it is a
  // composition).
  std::cout<<"The resource in A is still: ";
  A.resource().whoAmI();
  
  // It works also with assignement
  Container C;
  C=B;
  B.resource().setValue(100.);
  std::cout<<"Now the resource in B is: ";
  B.resource().whoAmI();
  std::cout<<"But, the resource in C is still: ";
  C.resource().whoAmI();
  // Move semantic is implemented
  C=std::move(A);
  // Now the resource of A is empty and C has stolen it
  // The resource previously contained in C has been deleted and memory freed
  std::cout<<"C has stolen resource from A: ";
  C.resource().whoAmI();
  // Clearly I can choose (also run time) the concrete type
  Container D;
  D.chooseResource(2);
  std::cout<<"The resource in D is: ";
  D.resource().whoAmI();


}

               
