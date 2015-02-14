#include "rk45.hpp"
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
  double errorDesired=1.e-4;
  int status;
  auto result= 
    rk45(fun,t0,T,y0,h_init,(T-t0)/4.,errorDesired,status,10000);
  ofstream file("result.dat");
  for (auto v : result)
    file<<v.first<<" "<<v.second<<std::endl;
	 
}
