#include<iostream>
#include "gcd.hpp"
int main()
{
  using namespace Utility;
  using namespace std;
  cout<<" Greatest common divisor between 399 and 120 is "<<Gcd<399,120>::value<<endl;
  // Just to show constexpr
  constexpr unsigned long n=399;
  constexpr unsigned long m=120;
  cout<< "Computed with the second implementation: "<<GCD(n,m)<<endl;
  //  cout<< "give me two integers"<<std::endl;
  //unsigned long int a,b;
  //std::cin>>a>>b;
  //! It works also if the arguments are not constant expressions!
  //cout<< "Computed with the second implementation: "<<GCD(a,b)<<endl;
  

}
