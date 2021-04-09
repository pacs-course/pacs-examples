#include<iostream>
#include <complex>
#include "monomials.hpp"
#include "polynomials.hpp"

int main()
{
  using namespace apsc::LinearAlgebra;
  using namespace std::complex_literals;
  auto x1 = monomial<3>(3.0); // 3^3
  auto x2 = monomial<4>(3.0+2.0i); // (3+2i)^4
  std::cout<< x1 <<" "<< x2 << std::endl;

  // A real polynomial
  Polynomial<4,double> p1{{0., 1., 2. ,3., 2.}};
  std::cout<<p1<<std::endl;
  std::cout<<p1(3.0)<<std::endl;
  // A complex polynomial
  std::array<std::complex<double>, 3> c{4.2+5.0i, -7.0 +0.3i, 0.0 -3.0i};
  Polynomial<2,std::complex<double>> p2{c};
  std::cout<<p2<<std::endl;
  std::cout<<p2(1.0+3.0i)<<std::endl;
  //double is converted to complex<double>
  std::cout<<p2(3.0)<<std::endl;
  
  //I can create a polynomial with integer coefficients
  Polynomial<3,int> pi{ std::array<int,4>{{1, 2,3, 4}} };
  std::cout<<pi<<std::endl;
  std::cout<< pi(9)<< std::endl;
  
  Polynomial<2,double> p4{{0., 1., 2. }};

  auto [quot,rest] = PolyDivide(p1,p4);
  std::cout<<p1<<"=\n";
  std::cout<<"("<<p4<<")*("<<quot<<") + "<<rest<<std::endl;

  auto p5= p1+3.0*p4 - p1*Polynomial<4,double>{{1.,2.,-5.,0.,1.}};
  std::cout<<"p5="<<p5<<std::endl;

  auto p6= pow<4>(p4);
  std::cout<<"p4^4="<<p6<<std::endl;

}
                               
 
                        
                              
                              
                               
                          
  

