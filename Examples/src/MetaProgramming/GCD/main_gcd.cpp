#include<iostream>
#include "gcd.hpp"
int main()
{
  using namespace Utility;
  using namespace std;
  cout<<" Greatest common divisor between 399 and 120 is "<<Gcd<399,120>::value<<endl;
  unsigned long n=399;
  unsigned long m=120;
  cout<< "Computed with recursive ordinary function: "<<gcd(n,m)<<endl;

  cout<< "Computed with the second implementation: "<<GCD<399,120>()<<endl;
}
