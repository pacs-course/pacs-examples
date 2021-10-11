/*
 * test_polyzero.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: forma
 */
#include "polyHolder.hpp"
#include <iostream>
#include <sstream>
int main()
{
  using namespace apsc;
  using Coefficients = PolyHolder::Coefficients;
  // seventth grade polynomial
  Coefficients coeff{
    {1.,0.},
    {-3,0.},
    {15.,0},
    {-3.5,0.},
    {5.,-0.},
    {1.,-0.},
    {-3.,0.},
   {5.,0.}
  };
  std::cout<<"Polynomial:\n";
  int exp{0};
  std::ostringstream s;
  for (auto c: coeff)
    s<<c<<"x^"<<exp++<<"+";
  auto ss=s.str();
  ss.pop_back();
  std::cout<<ss<<std::endl;

    PolyHolder holder(std::move(coeff));
  std::complex<double> x={1.,1.};
  auto y=polyEval(holder.pCoefficients(), x);
  std::cout<<"p("<<x<<")="<<y<<std::endl;


  auto der = holder.derivatives(x,5);
  std::cout<<"Derivatives at x= "<<x<<std::endl;
  std::cout<<" Computed with synthetic division or with the coefficients\n";
  std::cout<<"Order\t synt.div.\t\t coeff.\n";
  std::cout.precision(2);
  std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed
  unsigned int i{0u};
  for (auto v:der)
    {
      auto coeffDer=holder.derivativeCoeff(i);
      auto valDer = polyEval(coeffDer, x);
      std::cout<<i++<<"\t"<<v<<"\t"<<valDer<<std::endl;
    }
  auto [zeros,res,status]=
      polyRoots(holder.pCoefficients(),holder.pCoefficients().size()-1u, std::complex<double>{0.,1.});

  std::cout<<"Zeroes and residuals"<<std::endl;
  auto r =res.begin();
  std::cout.setf( std::ios::scientific, std:: ios::floatfield ); // floatfield set to scientific
  for (auto const & zero:zeros)
    {
      std::cout<<zero<<" "<<*(r++)<<std::endl;
    }


}


