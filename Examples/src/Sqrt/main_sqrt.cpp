#include "squareRoot.hpp"
#include <iostream>
#include <cmath>
int main()
{
  using std::cout;
  using std::endl;
  using std::sqrt;
  
  cout<<" Square root of 5.0"<<endl;
  cout<<"with squareRoot()="<<squareRoot(5.0)<<" ";
  cout<<"with sqrt()="<<sqrt(5.0)<<endl;

  cout<<" Square root of 56.0"<<endl;
  cout<<"with squareRoot()="<<squareRoot(5.0)<<" ";
  cout<<"with sqrt()="<<sqrt(5.0)<<endl;

  cout<<" Square root of -3"<<endl;
  cout<<"with squareRootC()="<<squareRootC(-3.0)<<endl;

}
