#include "generic_factory.hpp"

class Base
{
public:
  Base(const int &x_)
    : x(x_)
  {}

  virtual ~Base() = default;

  virtual void
  print() const = 0;

protected:
  int x;
};

class Derived1 : public Base
{
public:
  using Base::Base;

  virtual void
  print() const override
  {
    std::cout << "Derived 1, x = " << x << std::endl;
  }
};

class Derived2 : public Base
{
public:
  using Base::Base;

  virtual void
  print() const override
  {
    std::cout << "Derived 2, x = " << x << std::endl;
  }
};

int
main(int argc, char **argv)
{
  using Factory = GenericFactory<Base, const int &>;

  // Register derived classes into the factory.
  Factory::register_child<Derived1>("D1");
  Factory::register_child<Derived2>("D2");

  // Create a given instance and use it.
  const auto x = Factory::create_child("D2", -3);
  x->print();

  return 0;
}
