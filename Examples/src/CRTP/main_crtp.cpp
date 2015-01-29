#include "crtp.hpp"
#include<iostream>
// A template that uses pur class

int main()
{
  using std::cout;
  using std::endl;
  Base<Derived1> d1;
  Base<Derived2> d2;
  cout<<"Derived"<< Base<Derived1>::fstatic()<<" fun(5.0)="<<d1.fun(5.0)<<endl;
  cout<<"Derived"<< Base<Derived2>::fstatic()<<" fun(5.0)="<<d2.fun(5.0)<<endl;
}
