// a test for cloning utilitiess
#include "CloningUtilities.hpp"
#include <iostream>
#include <unordered_set>
// A nonclonable class
class NoClone
{
public:
  int a;
};

// A hierarchy of clonable classes
//! the base class
/*!
  I want the hiearchy to be clonable so I set clone() method.
 */
class Base
{
public:
  virtual void                  whoAmI() const = 0;
  virtual void                  setValue(double const &) = 0;
  virtual std::unique_ptr<Base> clone() const = 0;
  virtual ~Base() = default;
};

//! Derived class
class Derived1 : public Base
{
public:
  Derived1(double const x = 0.) : value_{x}
  {
    std::cout << "I am a new Derived1\n";
  };
  void
  setValue(double const &inp) override
  {
    value_ = inp;
  }
  void
  whoAmI() const override
  {
    std::cout << " I am a Derived1 and I store " << value_ << std::endl;
  }
  std::unique_ptr<Base>
  clone() const override
  {
    std::cout << "I am Derived1 and I have been cloned\n";
    return std::make_unique<Derived1>(*this);
  };

private:
  double value_ = 1.0;
};

class Derived2 : public Base
{
public:
  Derived2(double const x = 0.) : value_{x}
  {
    std::cout << "I am a new Derived2\n";
  };
  void
  setValue(double const &inp) override
  {
    value_ = inp;
  }
  void
  whoAmI() const override
  {
    std::cout << " I am a Derived2 and I store " << value_ << std::endl;
  }
  std::unique_ptr<Base>
  clone() const override
  {
    std::cout << "I am Derived2 and I have been cloned\n";
    return std::make_unique<Derived2>(*this);
  };

private:
  double value_ = 2.0;
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
  void
  chooseResource(int i)
  {
    if(i == 1)
      {
        resource_ = apsc::make_PointerWrapper<Base, Derived1>();
      }
    else
      {
        resource_ = apsc::make_PointerWrapper<Base, Derived2>();
      }
  }
  // ! I can access the resource by reference
  Base &
  resource()
  {
    return *resource_;
  }
  Base const &
  resource() const
  {
    return *resource_;
  }
  bool empty()const {return resource_==nullptr;}
private:
  apsc::PointerWrapper<Base> resource_;
};
// This will generate all non-template methods (to check if they compile!)
template class apsc::PointerWrapper<Base>;

int
main()
{
  //! Testing has_clone
  std::cout << std::boolalpha;
  std::cout << "In NoClone clonable?  "
            << apsc::TypeTraits::has_clone<NoClone>() << std::endl;
  std::cout << "In Derived2 clonable? "
            << apsc::TypeTraits::has_clone<Derived2>() << std::endl;
  std::cout << std::endl;
  Container A;
  A.chooseResource(1);
  A.resource().setValue(99.);
  std::cout << "The resource in A is: (should be Derived1)";
  A.resource().whoAmI();
  Container B{A}; // B contains A COPY of the resource of A
  std::cout << "The resource in B is: (should be Derived1)";
  B.resource().whoAmI();
  // I can change the value
  B.resource().setValue(55.);
  std::cout << "Now the resource in B is: (should be Derived1)";
  B.resource().whoAmI();
  // But in A is unchanged, since A own its own resource uniquely (it is a
  // composition).
  std::cout << "The resource in A is still: (should be Derived1)";
  A.resource().whoAmI();

  // It works also with assignement
  Container C;
  C = B;
  B.resource().setValue(100.);
  std::cout << "Now the resource in B is: (value changed)";
  B.resource().whoAmI();
  std::cout << "But, the resource in C is still: (same as old B)";
  C.resource().whoAmI();
  // Move semantic is implemented
  C = std::move(A);
  // Now the resource of A is empty and C has stolen it
  // The resource previously contained in C has been deleted and memory freed
  std::cout << "C has stolen resource from A: Is A empty? " << A.empty()
            << " Is C empty? " << C.empty() << std::endl;
  C.resource().whoAmI();
  // Clearly I can choose (also run time) the concrete type
  Container D;
  D.chooseResource(2);
  std::cout << "The resource in D is: (should be Derived 2)";
  D.resource().whoAmI();

  // Testing some part of PointerWrapper code

  apsc::PointerWrapper<Base> wb = apsc::make_PointerWrapper<Base, Derived1>();
  std::cout << "The resource in wb is: ";
  wb->whoAmI();
  wb = apsc::make_PointerWrapper<Derived2, Derived2>();
  std::cout << "The resource in wb is now (now is Derived2 and storing 0): ";
  wb->whoAmI();
  apsc::PointerWrapper<Derived1> wz = apsc::make_PointerWrapper<Derived1, Derived1>(4);
  std::cout << "The resource in wz is: (should be Derived1 and storing 4) ";
  wz->whoAmI();
  apsc::PointerWrapper<Base> wc =
    apsc::make_PointerWrapper<Derived1, Derived1>(55);
  std::cout << "The resource in wc is: (should be Derived1 and storing 55)";
  wc->whoAmI();
  apsc::PointerWrapper<Base> wk{wz};
  std::cout << "The resource in wk is: (should be Derived1 and identical to wz)";
   wk->whoAmI();
   wz->setValue(80);
   wk=wz;
   std::cout << "The resource in wk isnow: (should be Derived1 and store 80)";
   wk->whoAmI();

  std::cout << "testing some operators\n";

  std::cout << std::boolalpha << (wz == wc) << " " << (wz < wc) << std::endl;

  std::cout << " Testing the hash\n";
  std::unordered_set<apsc::PointerWrapper<Base>> sw{wb, wc, wz};
  for(auto const &z : sw)
    z->whoAmI();
}
