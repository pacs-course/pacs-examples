#include "nonLinSys.hpp"
#include <iostream>
//! A functor
struct f1{
  double operator ()(NonLinearSystems::argumentType const & x){
    return x[0]*x[1];
  }
};

//! A function

double f2(NonLinearSystems::argumentType const & x){
  return x[0]+3*x[1];
}

int main(){
  using namespace  NonLinearSystems;
  using std::vector;
  using std::cout;
  using std::endl;
  NonLinSys F;
  // fill the system
  /*
    having used a function wrapper we may treat functors, function pointers
    and lambda expressions in a similar way
   */
  F.addToSystem(f1());
  F.addToSystem(&f2);
  // Lambda expression added
  F.addToSystem( [](argumentType const & x){return x[0]+x[1];} );
  argumentType x(2);
  x<<1.0,3.1; // Eigen initialization
  auto res=F(x);
  cout<<res<<endl;
  

}
