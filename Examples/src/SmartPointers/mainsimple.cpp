//! @file mainsimple.cpp
//! @ brief A simple example of unique_ptr.
/*!
  It shows how an unique_ptr works.
 */
#include <iostream>
#include <memory>
class AbstractBase
{
public:
  AbstractBase() { std::cout << "Creating an AbstractBase" << std::endl; }
  //~AbstractBase(){ // Wrong, it should be virtual
  virtual ~AbstractBase()
  {
    std::cout << "Destroying an AbstractBase" << std::endl;
  }

private:
  virtual void dummy() = 0; // To make it abstract
};

class Derived1 : public AbstractBase
{
public:
  Derived1() { std::cout << "Creating a Derived1" << std::endl; }
  ~Derived1() { std::cout << "Destroying a Derived1" << std::endl; }

private:
  virtual void dummy() override{};
  double       aDouble;
};

class Derived2 : public AbstractBase
{
public:
  Derived2() { std::cout << "Creating a Derived2" << std::endl; }
  ~Derived2() { std::cout << "Destroying a Derived2" << std::endl; }

private:
  virtual void dummy() override{};
  double       aDouble;
};

enum class Select
{
  DERIVED1,
  DERIVED2
};

std::unique_ptr<AbstractBase>
buildAbstractBase(Select s)
{
  switch(s)
    {
    case Select::DERIVED1:
      return std::unique_ptr<Derived1>(new Derived1);
    case Select::DERIVED2:
      return std::unique_ptr<Derived2>(new Derived2);
    default:
      return std::unique_ptr<AbstractBase>(); // null pointer
    }
}

class Container
{
public:
  Container(Select s) : my_data(buildAbstractBase(s))
  {
    std::cout << "Creating a Container" << std::endl;
  }
  ~Container() { std::cout << "Destroying a Container" << std::endl; }
  void
  changeObject(Select s)
  {
    // Assignement is here possible
    // it trasfers ownership
    my_data = buildAbstractBase(s); // this is a move assignement!
  }

private:
  std::unique_ptr<AbstractBase> my_data;
};

int
main()
{
  Container a(Select::DERIVED1);
  a.changeObject(Select::DERIVED2);

  {
    std::cout << "Entering an enclosing scope" << std::endl;
    Container b(Select::DERIVED2);
    std::cout << "Exiting an enclosing scope" << std::endl;
  }
}
