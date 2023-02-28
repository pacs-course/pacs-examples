//
// Created by forma on 24/02/23.
//
#include <iostream>
#include <vector>
#include <cmath>
#include "tridiagonalSystem.hpp"

using namespace apsc::LinearAlgebra;
int main()
{
  std::cout<<" Creating a simple tridiagonal matrix\n";
  std::size_t n=20;
  std::vector<double> a(n,5.);
  std::vector<double> b(n,-1.);
  std::vector<double> c(n,-0.5);
  std::vector<double> exact(n,1.);
  auto rhs = matVecTrid(a,b,c,exact);
  auto sol = thomasSolve(a, b, c, rhs);
  double error=0.;
  for (auto i=0u;i<sol.size();++i)
  error = std::max(error, std::abs(sol[i]-exact[i]));
  std::cout<<"System solved, error="<<error<<std::endl;
  std::cout<<" Now we make the system periodic\n";
  rhs = matVecTrid(a,b,c,exact,true);
  sol = thomasSolveSym(a, b, c, rhs);
  error=0.;
  for (auto i=0u;i<sol.size();++i)
  error = std::max(error, std::abs(sol[i]-exact[i]));

  std::cout<<"Periodic system solved, error="<<error<<"sol.front()-sol.back()="<<(sol.front()-sol.back())<<std::endl;


}

