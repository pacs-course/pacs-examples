#include "crtp.hpp"
#include<iostream>
// A function that calls Base<T>.fun
template <class T>
double callFun5(T const & a)
{
  return a.fun(5.0);
}
int main()
{
  using std::cout;
  using std::endl;
  Base<Derived1> d1;
  Base<Derived2> d2;
  cout<<"Derived"<< Base<Derived1>::fstatic()<<" fun(5.0)="<<callFun5(d1)<<endl;
  cout<<"Derived"<< Base<Derived2>::fstatic()<<" fun(5.0)="<<callFun5(d2)<<endl;
}
