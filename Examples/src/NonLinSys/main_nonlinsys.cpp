#include "newton.hpp"
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
  cout<<" Residual at ("<<x<<")=("<<res<<")"<<endl;
  cout<<" Jacobian at ("<<x<<")=["<<endl;
  cout<< DiscreteJacobian(&F)(x)<<"]"<<endl;
  /// Another non linear system
  NonLinSys G;
  G.addToSystem([](argumentType const & x){return x[0]+x[1]+x[2];});
  G.addToSystem([](argumentType const & x){return x[0]*x[0]+3*x[1]-x[2];});
  G.addToSystem([](argumentType const & x){return x[0]+2*x[1]+7*x[2];});
  DiscreteJacobian J(&G);
  NewtonOptions optNewton{1e-6,1.e-8,100};
  argumentType y(3);
  y[0]=1;y[1]=1;y[2]=1;
  NewtonStatus result=Newton(G,J,y,optNewton);
  cout<<"Residual="<<result.residual<<" Iterations= "<<result.iterations;
  cout<<" Converged="<<result.converged<<endl;
  cout<<"Solution= ("<<y<<")"<<endl;
}
