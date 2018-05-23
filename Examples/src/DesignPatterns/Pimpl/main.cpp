#include<iostream>
// The user uses only api.hpp!
#include "api.hpp"
int main ()
{
  API::user_interface api;
  api.get_a()=5.0;
  std::cout<<api.get_a()<<std::endl;
  
}
