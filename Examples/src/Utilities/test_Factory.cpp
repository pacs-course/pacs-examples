//! A simple example of use of the generic object Factory template class
#include "Factory.hpp"
#include "Proxy.hpp"
#include <iostream>
// an abstract class
class Abstract
{
public:
  virtual void whoAmI() = 0;
  virtual ~Abstract() = default;
};

//! Some conclrete classes
/*!@{ */
class Derived1 : public Abstract
{
public:
  void
  whoAmI() override
  {
    std::cout << " I am Derived1" << std::endl;
  }
};

class Derived2 : public Abstract
{
public:
  void
  whoAmI() override
  {
    std::cout << " I am Derived2" << std::endl;
  };
};

class Derived3 : public Abstract
{
public:
  void
  whoAmI() override
  {
    std::cout << " I am Derived3" << std::endl;
  };
};
/*!@} */

using Identifier = int;
using Builder = std::function<std::unique_ptr<Abstract>()>;
using MyObjectFactory = GenericFactory::Factory<Abstract, Identifier, Builder>;

// Defining the builders for the different concrete objects
Builder build1 = [] { return std::make_unique<Derived1>(); };
Builder build2 = [] { return std::make_unique<Derived2>(); };
Builder build3 = [] { return std::make_unique<Derived3>(); };

//! Normally this is done elsewhere, but this is only a test
/*!
  Filling the first 2 factory with the builders
 */
void
loadFactory()
{
  auto &factory = MyObjectFactory::Instance();
  factory.add(1, build1);
  factory.add(2, build2);
}
// the third with the proxy
namespace
{
GenericFactory::Proxy<MyObjectFactory, Derived3> P3{3, build3};
}

int
main()
{
  // fill factory
  loadFactory();
  // get the factory
  auto &factory = MyObjectFactory::Instance();
  auto  list = factory.registered();
  std::cout << "Registered identifiers:" << std::endl;
  for(std::size_t i = 0; i < list.size(); ++i)
    {
      std::cout << list[i] << std::endl;
    }
  std::cout << " Type identifier of desired object (1,2,3)" << std::endl;
  Identifier i;
  std::cin >> i;
  auto object = factory.create(i);
  object->whoAmI();
}
