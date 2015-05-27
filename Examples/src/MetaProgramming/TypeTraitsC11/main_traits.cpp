#include <iostream>
#include <iomanip>
#include<vector>
#include "isDerived.hpp"
#include "fselect.hpp"
#include "move_swap.hpp"
class B{
public:
  bool a;
  B()=default;
  B(B const & b):a{b.a}
  {
    std::cout<<" Copy construct B "<<std::endl;
  }
  B(B&& b):a{std::move(b.a)}
  {
    std::cout<<" Move construct B "<<std::endl;
  }
  B & operator = (B const & b)
  {
    std::cout<<" Copy assign B "<<std::endl;
    a=b.a;
    return *this;
  }
  B & operator = (B&& b)
  {
    std::cout<<" Move assign B "<<std::endl;
    a=std::move(b.a);
    return *this;
  }
};
class D: public B
{
public:
  D()=default;
  D(D const & b):B(b)
  {
    std::cout<<" Copy construct D "<<std::endl;
  }
  D & operator = (D const & b)
  {
    std::cout<<" Copy assign D "<<std::endl;
    a=b.a;
    return *this;
  }
  // Move constructor and assignment not viable because I have explicitely
  // declared the copy equivalent ones.
};

class C {};

class E : private B{};

int main(){
  convertible_t<B,D>();
  //convertible_t<B,C>(); //ERROR
  std::cout<<std::boolalpha<<" D derives publicly from B? Answer="<<IsDerived<B,D>::value<<std::endl;
  std::cout<<std::boolalpha<<" C derives publicly from B? Answer="<<IsDerived<B,C>::value<<std::endl;
  std::cout<<std::boolalpha<<" E derives publicly from B? Answer="<<IsDerived<B,E>::value<<std::endl;

  std::vector<double> a{2.,3.,4.};
  double result(0);
  for (auto i : a) result+=inv(i);
  std::cout<<"Result= "<<result<<std::endl;
  double *ap = a.data();
  std::vector<double*> b;
  for( auto i=0u; i<a.size();++i,++ap)b.emplace_back(ap);
  result=0;
  for (auto i : b) result+=inv(i);
  std::cout<<"Resu;t= "<<result<<std::endl;
  B b1,b2;
  D d1,d2;
  // Swapping movable object
  Swap(b1,b2);
  // Swapping non movable objects
  Swap(d1,d2);

}
