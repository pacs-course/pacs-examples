//
//! @brief  A test of the ckass Chrono and of polynomial evaluation
//! @detail We compare the evaluation of a polygon with the standard rule
//!         against Horner's rule.

#include <iostream>
#include <vector>
#include <cmath>
#include "chrono.hpp"
#include "horner.hpp"

int main() {
  using namespace std;
  using namespace Timings;


  vector<double>a;
  int n;
  cout<<"give me polynomial degree"<<endl;
  cout<<"=>";
  cin >>n;
  a.reserve(n+1);
  cout<<"Coefficients are computed automatically"<<endl;
  for(int i=0;i<=n;++i)a.push_back(2*std::sin(2.0*i));
  /*
  cout<<"Give me coefficients a0, a1..."<<endl;
  for(int i=0;i<=n;++i){
    double temp;
    cout<<"a["<<i<<"]=";
    cin>>temp;
    cout<<endl;
    a.push_back(temp);
  }
  */
  double startx=0.00;
  double endx=1.00;
  double interval=0.5e-6;
  int numinterval=(endx-startx)/interval; // truncated if not integer!
  cout<<"Computing "<<numinterval<<" evaluation of polynomial"
      <<" with standard formula"<<endl;
  
  vector<double> points;
  points.reserve(numinterval+1);
  double x=startx;
  for (int i=0; i <= numinterval; ++i)
  {
    points.push_back(x);
    x+=interval;
  }
  Chrono temporizzatore;
  temporizzatore.start();
  evaluatePoly(points,a,&eval);
  cout<<endl;
  temporizzatore.stop();
  
  cout<<temporizzatore<<endl;

  cout<<"Computing "<<numinterval<<" evaluation of the polynomial with"
      <<" Horner's rule"<<endl;
  temporizzatore.start();
  evaluatePoly(points,a,&horner);
  cout<<endl;
  temporizzatore.stop();
  cout<<temporizzatore<<endl;
}  // end of main()


