#include <iostream>
#include "rational.hpp"

//===============================================
int main()
{
  using namespace std;
  cout <<"Size of a Rational="<<sizeof(Rational)<<" bytes"<<endl;
  Rational a(-15,6);
  Rational b(6);
  Rational c(b);
  Rational d;
  c=static_cast<Rational>(1);
  double aa=a;
  cout<<a<<endl;
  cout<<++a<<endl;
  cout<<b<<endl;
  cout<<c<<endl;
  cout<<aa<<endl;
  cout<<a+b<<endl;
  cout<<a+b<<endl;
  cout<<a+1<<endl; // it converts a to double
  cout<<a+Rational(1)<<endl; // explicit conversion!

  std::ratio<3,4> threeQuarter;
  Rational tq(threeQuarter);
  cout<< tq<<endl;
  Rational mu=std::micro();
  cout<<" A micro is "<<mu<<endl;

  cout<<" Give me a Rational in the form a/b (followed by a space):";
  cin>>a;
  cout<<endl<<"I have read "<<a<<endl;
}


