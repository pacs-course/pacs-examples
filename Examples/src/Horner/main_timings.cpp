//
//! @brief  A test of the ckass Chrono and of polynomial evaluation
//! @detail We compare the evaluation of a polygon with the standard rule
//!         against Horner's rule.

#include <vector>
#include <cmath>
#include "chrono.hpp"
#include "horner.hpp"

int main() {
  using namespace std;
  using namespace Timings;


  vector<double>a;
  int n;
  cout<<"dammi il grado del polinomio"<<endl;
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
  double interval=1.0e-6;
  int numinterval=int((endx-startx)/interval);
  cout<<"Computing "<<numinterval<<" evaluation of polynomial"
      <<" with standard formula"<<endl;
  
  vector<double> points;
  points.reserve(numinterval+1);
  double x=startx;
  for (int i=0; i <= numinterval; ++i,x+=interval)points.push_back(x);
  // to make the game fair I fill the vector results
  // so both methods start in thw same situation
  vector<double> result(points.size());
  Chrono temporizzatore;
  temporizzatore.start();
  evaluatePoly(points,a,result,&eval);
  cout<<endl;
  temporizzatore.stop();
  
  cout<<temporizzatore<<endl;

  cout<<"Computing "<<numinterval<<" evaluation of the polynomial with"
      <<" Horner's rule"<<endl;
  temporizzatore.start();
  evaluatePoly(points,a,result,&horner);
  cout<<endl;
  temporizzatore.stop();
  cout<<temporizzatore<<endl;
}  // end of main()


