#include "IsClonable.hpp"
#include <iostream>
#include <iomanip>
struct C
{
  C * clone(){return new C(*this);}
  int i;
};

struct NC
{
  int i;
};

int main()
{
  using namespace TypeTraits;
  std::cout<<"C is clonable:  "<<std::boolalpha<<isClonable<C>()<<std::endl;
  std::cout<<"NC is clonable: "<<std::boolalpha<<isClonable<NC>()<<std::endl;
}

