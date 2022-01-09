/*
 * test_polyzero.cpp
 *
 *  Created on: Sep 24, 2021
 *      Author: forma
 */
#include "polyHolder.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <tuple>
int main()
{
  using namespace apsc;
  using Coefficients = PolyHolder::Coefficients;
  /* 
  Coefficients coeff{
    {1.,0.},
    {-3,1.},
    {15.,0},
    {-3.5,0.},
    {5.,-1.},
    {5.,0.}
  };
  */
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

    PolyHolder holder(coeff);
  std::complex<double> x={1.,1.};
  auto y=polyEval(holder.pCoefficients(), x);
  std::cout<<"p("<<x<<")="<<y<<std::endl;


  auto der = holder.derivatives(x,5);
  std::cout<<"Testing computation of Derivatives at x= "<<x<<std::endl;
  std::cout<<" Computed with synthetic division or with the coefficients\n";
  std::cout<<"Order\t synt.div.\t\t coeff.\n";
  std::cout.precision(2);

  std::cout.setf( std::ios::fixed|std::ios::showpos, std:: ios::floatfield ); // floatfield set to fixed
  unsigned int i{0u};
  for (auto v:der)
    {
      auto coeffDer=holder.derivativeCoeff(i);
      auto valDer = polyEval(coeffDer, x);
      std::cout<<i++<<"\t"<<v<<"\t\t"<<valDer<<std::endl;
    }
  std::cout<<"Testing zeros of  polynomial"<<std::endl;
// If i want to find complex zeros the initial point must have non null imaginary part!

  auto [zeros,res,status]=
      polyRoots(coeff,coeff.size()-1, std::complex<double>{0.,1.});

  std::cout<<"Zeroes and residuals"<<std::endl;
  std::cout<<"Zero\t\tResidual\t\t|p(zero)|\n";
  auto r =res.begin();
  std::cout.setf( std::ios::scientific, std:: ios::floatfield ); // floatfield set to scientific
  std::cout.width(15);
  for (auto const & zero:zeros)
    {
      auto restrue=std::abs(polyEval(coeff,zero));
      std::cout<<zero<<"\t\t"<<*(r++)<<"\t\t"<<restrue<<std::endl;
    }

  std::cout<<"Testing zeros of  polynomial with complex coefficients:"<<std::endl;
  coeff[1]={2.,-2.};
  coeff[2]={0.,-1.};
  exp=0;
  s.str("");
  for (auto c: coeff)
    s<<c<<"x^"<<exp++<<"+";
  ss=s.str();
  ss.pop_back();
  std::cout<<ss<<std::endl;

  std::tie(zeros,res,status)=
      polyRoots(coeff,coeff.size()-1, std::complex<double>{0.,1.});
  r =res.begin();
  std::cout.setf( std::ios::scientific, std:: ios::floatfield ); // floatfield set to scientific
  std::cout<<"Zero\t\tResidual\t\t|p(zero)|\n";
  for (auto const & zero:zeros)
    {
      auto restrue=std::abs(polyEval(coeff,zero));
      std::cout<<zero<<"\t\t"<<*(r++)<<"\t\t"<<restrue<<std::endl;
    }

  std::cout<<"\nNow a case with a multiple zero (equal to 1):"<<std::endl;
    coeff.resize(4);
    coeff[0]={-1.,0.};
    coeff[1]={ 3.,0.};
    coeff[2]={-3.,0.};
    coeff[3]={ 1.,0.};
    exp=0;
    s.str("");
    for (auto c: coeff)
      s<<c<<"x^"<<exp++<<"+";
    ss=s.str();
    ss.pop_back();
    std::cout<<ss<<std::endl;

    std::tie(zeros,res,status)=
        polyRoots(coeff,coeff.size()-1, std::complex<double>{0.,1.});
    r =res.begin();
    std::cout.setf( std::ios::scientific, std:: ios::floatfield ); // floatfield set to scientific
    std::cout<<"Zero\t\tResidual\t\t|p(zero)|\n";
    for (auto const & zero:zeros)
      {
        auto restrue=std::abs(polyEval(coeff,zero));
        std::cout<<zero<<"\t\t"<<*(r++)<<"\t\t"<<restrue<<std::endl;
      }







}


