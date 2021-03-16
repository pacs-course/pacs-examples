/*
 * main_Richardson.cpp
 *
 *  Created on: Mar 15, 2021
 *      Author: forma
 */
#include "Richardson.hpp"
#include "CrankNicolson.hpp"
#include "chrono.hpp"
#include <fstream>
int main()
{
  auto f=[](double y,double t){return -y*y;};
  double t0=0;
  double T=5.0;
  double y0=1.;
  unsigned n =1000;
  Timings::Chrono clock;
  clock.start();
  auto [time,sol]=apsc::CrankNicolson(f, y0, t0, T, n);
  clock.stop();
  double constexpr exact=0.166666667;
  std::cout<<"Exact solution "<<exact<<std::endl;
  std::cout<<"CN solution on "<<n<<" subintervals="<<sol.back()<<std::endl;
  std::cout<<"Error = "<<std::abs(sol.back()-exact)<<std::endl;
  std::cout<<"obtained in time "<<clock<<std::endl;
  auto r = [&t0,&T,&f,&y0](double const & h)
            {
    unsigned n =(T-t0)/h;
    std::vector<double> sol;
    std::tie(std::ignore,sol) = apsc::CrankNicolson(f, y0, t0, T, n);
    return sol.back();
            };
  constexpr unsigned M=4;
  std::vector<unsigned> k(M);
  for (unsigned i=0;i<M;++i) k[i]=i+2;

  apsc::LinearAlgebra::Richardson richardson{r,k};
// passing the lambda expression directly  
//  apsc::LinearAlgebra::Richardson<decltype(r)> richardson{r,k};
  unsigned N = 4;
  double h = (T-t0)/N;
  clock.start();
  auto solR= richardson(h);
  clock.stop();
  std::cout<<"solution with Richardson with "<<M<<" terms="<<solR<<std::endl;
  std::cout<<"obtained in time "<<clock<<std::endl;
  std::cout<<"with error estimate="<<richardson.getError()<<std::endl;
  std::cout<<"Actual Error="<<std::abs(solR-exact)<<std::endl;
  std::cout<<"Richardon iterates:\n";
  auto iterates=richardson.getIterates();
  for (auto v: iterates)
    std::cout<<v<<" Error="<< std::abs(v-exact)<< std::endl;
}



