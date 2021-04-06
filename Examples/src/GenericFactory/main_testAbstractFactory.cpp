//! A simple example of use of the generic object Factory template class
#include "Factory.hpp"
#include "Proxy.hpp"
#include <iostream>
#include <cmath>
// an abstract class
class Abstract
{
public:
  virtual void whoAmI()=0;
  virtual ~Abstract()=default;
};

//! Some concrete classes
/*!@{ */
class Derived1: public Abstract
{
public:
  void whoAmI() override
  {
    std::cout<<" I am Derived1"<<std::endl;
  }
};

class Derived2: public Abstract
{
public:
  void whoAmI() override
  {
    std::cout<<" I am Derived2"<<std::endl;
  };
};

class Derived3: public Abstract
{
public:
  void whoAmI() override
  {
    std::cout<<" I am Derived3"<<std::endl;
  };
};
/*!@} */

using Identifier=int;
using Builder=std::function<std::unique_ptr<Abstract>()>;
using MyObjectFactory=GenericFactory::Factory<Abstract,Identifier,Builder>;

// Defining the builders for the different concrete objects
Builder build1=[]{return std::make_unique<Derived1>();};
Builder build2=[]{return std::make_unique<Derived2>();};
Builder build3=[]{return std::make_unique<Derived3>();};


  
//! Normally this is done elsewhere, but this is only a test
/*! 
  Filling the first 2 factory with the builders 
 */
void loadFactory()
{
  auto & factory=MyObjectFactory::Instance();
  factory.add(1,build1);
  factory.add(2,build2);
}
// the third with the proxy
namespace
{
  GenericFactory::Proxy<MyObjectFactory,Derived3> P3{3,build3};
}
  
//! now an example of using the Factory with function (an alternative to the C-linkage)

using FunType= std::function<double (const double &, const double &)>;
using FunFactory=GenericFactory::FunctionFactory<Identifier,FunType>;
//! loads functions (normally in a separate file)
void loadFunctions()
{
  auto & Ffactory=FunFactory::Instance();

  Ffactory.add(1,[](const double & a, const double & b){return a*b;});
  // incomplete elliptic integral of the first kind, from the STL cmath lib (since c++17);
  // note I need to specify template argument explicitely to get the correct overload
  Ffactory.add(2,&std::ellint_1<double,double>);
  //atan2 form std: note I need to specify template argument explicitely to get the correct overload
  Ffactory.add(3,&std::atan2<double,double>);
}




int main()
{
  // fill factory
  loadFactory();
  // get the factory
  auto & factory=MyObjectFactory::Instance();
  auto list =factory.registered();
  std::cout<<"Registered identifiers:"<<std::endl;
  for (std::size_t i=0; i<list.size();++i)
    {
      std::cout<<list[i]<<std::endl;
    }
  std::cout<<" Type identifier of desired object (1,2,3)"<<std::endl;
  Identifier i;
  std::cin>>i;
  auto object = factory.create(i);
  object->whoAmI();
  // Now the functions
  loadFunctions();
  auto & ffactory=FunFactory::Instance();
  std::cout<<" \nType identifier of desired function (1,2,3)"<<std::endl;
  std::cin>>i;
  // get the function from the factory
  FunType f =ffactory.get(i);
  std::cout<<" Type x and y "<<std::endl;
  double x, y;
  std::cin>>x>>y;
  std::cout<<"f_"<<i<<"("<<x<<", "<<y<<")="<<f(x,y)<<std::endl;
}
