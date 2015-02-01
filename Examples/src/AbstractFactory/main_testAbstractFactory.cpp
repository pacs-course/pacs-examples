#include "Factory.hpp"
#include "Proxy.hpp"
#include <iostream>
#include <string>
//! I test the factory and the proxy

class Base
{
public:
  virtual ~Base(){};
  virtual void showMe()=0;
};

class Derived1 final: public Base 
{
public:
  void showMe(){std::cout<<" I Am Derived1"<<std::endl;}
};

class Derived2 final: public Base
{
public:
  void showMe(){std::cout<<" I Am Derived2"<<std::endl;}
};

// I use the standard builder
using MyFactory=GenericFactory::Factory<Base,std::string>;

template<class C>
using MyProxy=GenericFactory::Proxy<MyFactory,C>;

// Register through proxy

MyProxy<Derived1> dummy1("Derived1");
MyProxy<Derived2> dummy2("Derived2");

int main()
{
  using namespace std;
  string name;
  // Get the factory
  MyFactory & factory=MyFactory::Instance();
  while(true)
    {
      cout<<" What do you want? Derived1 or Derived2? (type quit to finish)"<<endl;
      cin>>name;
      if(name==string("quit")) return 0;
      try
	{
	  auto p = factory.create(name);
	  p->showMe();
	}
      catch (std::exception & e)
	{
	  cerr<<e.what()<<endl;
	  cerr<<"Allowed names:"<<endl;
	  for (auto i : factory.registered()) cerr<<i<<endl;
	}
    }
}
