#include<iostream>
#include <complex>
#include "monomials.hpp"
#include "polynomials.hpp"

int main()
{
  using namespace LinearAlgebra;
  
  auto x1 = monomial<3>(3.0); // 3^3
  auto x2 = monomial<4>(std::complex<double>{3,2}); // (3+2i)^4
  std::cout<< x1 <<" "<< x2 << std::endl;

  // A real polynomial
  Polynomial<4,double> p1{std::array<double,5>{{0., 1., 2. ,3., 2.}} };
  std::cout<<p1(3.0)<<std::endl;
  std::cout<<p1(std::complex<double>{0,3.0})<<std::endl;
  // A complex polynomial
  std::array<std::complex<double>, 3> c=
    {{
      {0.,1.},{1.,0.},{2.,2.}
      }};
  Polynomial<2,std::complex<double>> p2{c};
  std::cout<<p2(std::complex<double>{1.,3.0})<<std::endl;
  //This is  not possible becouse there is no conversion double-<complex<double>
  //std::cout<<p2(3.0)<<std::endl;
  
  //I can create a polynomial with integer coefficients
  Polynomial<3,int> pi{ std::array<int,4>{{1, 2,3, 4}} };
  std::cout.setf(std::ios_base::scientific);
  std::cout<<pi(9.0)<< " is a double while "<< pi(9)<< " is an int\n";
  
}
                               
 
                        
                              
                              
                               
                          
  

