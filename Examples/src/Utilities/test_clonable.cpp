#include "clonable.hpp"
#include <iostream>
struct Base1{
  Base1(double a=0.,int b=0):a{a},b{b}{}
  virtual ~Base1()=default;
  virtual double get_a()const {return this->a;}
  virtual int get_b()const {return this->b;}
  protected:
  double a;
  int b;
};
class Derived1: public Base1, public clonable<Base1,Derived1>
{
public:
  Derived1(double a, int b):Base1{a,b}{};
};
class Derived2: public Base1, public clonable<Base1,Derived2>
{
public:
  Derived2(){};
};
// this causes an error wron is not derived from Derived2
//class Wrong: public Base 1, clonable<Derived2>
//{
//};
int main()
{
  Derived1 a(3.9,5);
  Derived2 b;
  auto pa=a.clone();
  auto pb=b.clone();
  std::cout<<pa->get_a()<<" "<<pa->get_b()<<std::endl;
  std::cout<<pb->get_a()<<" "<<pb->get_b()<<std::endl;
}
