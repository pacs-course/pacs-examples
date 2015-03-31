#include "rk45.hpp"
#include<iostream>
#include <fstream>

#include <cmath>
int main()
{
  using namespace std;
  using namespace ODE;
  //  auto fun = [](double const & t, double const & y){return -10*y;};
  auto fun = [](double const & t, double const & y){return -std::sin(t);};
  double t0=0;
  double y0=1;
  double T=100;
  double h_init=0.2;
  double errorDesired=1.e-7;
  int status;
  auto result= 
    rk45(fun,t0,T,y0,h_init,(T-t0)/4.,errorDesired,status,10000);
  ofstream file("result.dat");
  for (auto v : result)
    file<<v.first<<" "<<v.second<<std::endl;
  file.close();

  // Only if I know the exact solution
  //auto exact=[](double const &t){return std::exp(-10.*t);}
  auto exact=[](double const &t){return std::cos(t);};
  double max_error(0);
  for (auto i : result)
    max_error=std::max(max_error,std::abs(exact(i.first)-i.second));
  std::cout.setf(std::ios::scientific);
  std::cout<<"Max error "<<max_error<<" Desired max error "<<errorDesired;
  std::cout<<std::endl;
}
