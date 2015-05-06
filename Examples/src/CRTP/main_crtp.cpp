#include "crtp.hpp"
#include "functions.hpp"
#include<iostream>
// A template that uses pur class

int main()
{
  using std::cout;
  using std::endl;
  Base<Derived1> d1;
  Base<Derived2> d2;
  cout<<"Derived"<< Base<Derived1>::fstatic()<<" fun(5.0)="<<myfun(d1)<<endl;
  cout<<"Derived"<< Base<Derived2>::fstatic()<<" fun(5.0)="<<myfun(d2)<<endl;
}
