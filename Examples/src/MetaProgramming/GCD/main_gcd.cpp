#include<iostream>
#include "gcd.hpp"
int main()
{
  using namespace Utility;
  using namespace std;
  cout<<" Greatest common divisor between 399 and 120 is "<<Gcd<399,120>::value<<endl;
  cout<< "Computed with recursive ordinary function: "<<gcd(399,120)<<endl;
}
