#include<iostream>
#include <fstream>
#include "RKF.hpp"
#include <cmath>
int main()
{
  using namespace std;
  using namespace apsc;
  auto fun = [](double const & t, double const & y){return -10*y;};
  auto exact=[](double const &t){return std::exp(-10.*t);};
  //auto exact=[](double const &t){return std::cos(t);};
  //auto fun = [](double const & t, double const & y){return -std::sin(t);};
  double t0=0;
  double y0=1;
  double T=10;
  double h_init=0.2;
  double errorDesired=1.e-4;
  {
    apsc::RKF<RKFScheme::RK45_t,RKFKind::SCALAR> solver{fun};
    //RKF<RKFScheme::RK23_t> solver{fun};
    //RKF<RKFScheme::RK12_t> solver{fun};
    auto solution = solver(t0,T, y0, h_init, errorDesired);
    double max_error=0.;
    int count=0;
    
    for (auto i : solution.y)
      max_error=std::max(max_error,std::abs(exact(solution.time[count++])-i));
    std::cout<<std::boolalpha;
    std::cout<<"effective Max error "<<max_error<<" Desired max error "<<errorDesired<<" Failed:"<<solution.failed<<
      " Estimated Error "<<solution.estimatedError<<"\n Contractions="<<solution.contractions<<" Expansions="<<solution.expansions;
    std::cout<<std::endl;
    
    //auto solution = solver(t0,T, y0, h_init, errorDesired);
    ofstream file2("result2.dat");
    file2<<solution;
  }
  {
    //Van Der Pol with mu=1
    auto fun=[](double const & t, Eigen::VectorXd y) ->Eigen::VectorXd
                         {
                           constexpr double mu=1.;
                           Eigen::VectorXd out(2);
                           out(0)= y(1);
                           out(1)=-y(0)+mu*(1-y(0)*y(0))*y(1);
                           return out;
                         };
    //apsc::RKF<RKFScheme::RK45_t,RKFKind::VECTOR> solver{fun};
    apsc::RKF<RKFScheme::ESDIRK34_t,RKFKind::VECTOR> solver{RKFScheme::ESDIRK34,fun};
    //apsc::RKF<RKFScheme::ESDIRK12_t,RKFKind::VECTOR> solver{fun};
    t0=0;
    T=100.;
    Eigen::VectorXd y0(2);
    y0[0]=1.;
    y0[1]=1.;
    int maxSteps=2000;
    errorDesired=1.e-1;
    auto solution = solver(t0,T, y0, h_init, errorDesired,maxSteps);
    std::cout<<" Desired max error "<<errorDesired<<" Failed:"<<solution.failed<<
       " Estimated Error "<<solution.estimatedError<<"\n Contractions="<<solution.contractions<<" Expansions="<<solution.expansions;
     std::cout<<std::endl;
     ofstream file3("resultVDP.dat");
     file3<<solution;

  }
  {
  //Verystiff with mu=1000
   auto fun=[](double const & t, Eigen::VectorXd y) ->Eigen::VectorXd
                        {
                          constexpr double mu=1000.;
                          Eigen::VectorXd out(2);
                          out(0)= std::cos(t)-y(0) +0.2*y(1);
                          out(1)=std::sin(t)-mu*y(1)+0.1*y(0);
                          return out;
                        };
   //apsc::RKF<RKFScheme::RK45_t,RKFKind::VECTOR> solver{fun};
   apsc::RKF<RKFScheme::ESDIRK34_t,RKFKind::VECTOR> solver{fun};
   //apsc::RKF<RKFScheme::ESDIRK12_t,RKFKind::VECTOR> solver{fun};
   t0=0;
   T=40.;
   Eigen::VectorXd y0(2);
   y0[0]=1.;
   y0[1]=1.;
   int maxSteps=200000;
   errorDesired=0.001;
   h_init=1.e-3;
   auto solution = solver(t0,T, y0, h_init, errorDesired,maxSteps);
   std::cout<<" Desired max error "<<errorDesired<<" Failed:"<<solution.failed<<
      " Estimated Error "<<solution.estimatedError<<"\n Contractions="<<solution.contractions<<" Expansions="<<solution.expansions;
    std::cout<<std::endl;
    ofstream file3("resultstiff.dat");
    file3<<solution;

 }



  return 0;
}
