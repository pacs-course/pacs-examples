#include<iostream>
#include "Pow.hpp"
int main(){
  double x=3.8;
  std::cout<<Pow<3>::apply(x)<<std::endl;
}
