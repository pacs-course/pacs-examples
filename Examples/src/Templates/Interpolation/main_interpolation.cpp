#include <iostream> 
#include <cmath> // for function exp  
#include "interpolation.hpp"
int main() {
  using namespace FunctionApproximation;
  using std::cout;
  using std::endl;
  using std::exp;
  std::cout.precision(6);
  std::cout.setf(std::ios::scientific);
  const int n = 4;
  std::vector<double> px(n);
  std::vector<double> py(n);

  // px[0] = 5; px[1] = 7; px[2] = 6; px[3] = 6.6;
  // py[0] = 1; py[1] = -23; py[2] = -50; py[3] = -4;

  for (int i = 0; i < n; i++) {
    px[i] = 1 + i/4.0; py[i] = exp( px[i] );
  }
  
  std::vector<double> xc={1, 1.3, 1.9, 3.5};
  
  auto resultl=interp1D(px,py,xc);
  NewtonInterpolator<double> nInter(px,py);
  auto resultn=interp1D(px,py,xc,nInter);
  
  // compute error
  std::vector<double> errorl(xc.size());
  for (unsigned int i=0;i<xc.size();++i)
    errorl[i]=std::exp(xc[i])-resultl[i];
  
  std::vector<double> errorn(xc.size());
  for (unsigned int i=0;i<xc.size();++i)
    errorn[i]=std::exp(xc[i])-resultn[i];
  
  cout<<"X   Lagr   Newt  ErrLagr  ErrNewt"<<endl;
  for (unsigned int i=0;i<xc.size();++i){
    cout<<xc[i]<<" "<<resultl[i]<<" "<<resultn[i]<<" "<<
      errorl[i]<<" "<<errorn[i]<<endl;
  }
  
}
