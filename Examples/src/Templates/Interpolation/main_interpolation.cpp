#include <iostream> 
#include <cmath> // for function exp  
#include "interpolation.hpp"
int main() {
  using namespace apsc;
  using namespace apsc::interpolationPolicy;
  using std::cout;
  using std::endl;
  using std::exp;
  std::cout.precision(5);
  std::cout.setf(std::ios::scientific);
  std::cout.setf(std::ios::left, std::ios::adjustfield);
  const int n = 10;
  std::vector<double> px(n);
  std::vector<double> py(n);

  // px[0] = 5; px[1] = 7; px[2] = 6; px[3] = 6.6;
  // py[0] = 1; py[1] = -23; py[2] = -50; py[3] = -4;
  auto fun = [] (double const & x){return std::sin(x);};
  // x coord between 1 and 6
  for (int i = 0; i < n; i++) {
    px[i] = 1 + i*6.0/(n-1); py[i] = fun( px[i] );
  }
  // the interpolators
  LagrangeInterpolator lagr(px,py);
  NewtonInterpolator  newt(px,py);
  PiecewiseLinearInterpolator plin(px,py);
  std::vector<double> xc={1, 1.3, 1.9, 3.5, 8.0};
  
  auto resultl=interp1D(xc,lagr);
  auto resultn=interp1D(xc,newt);
  auto resultpl=interp1D(xc,plin);

  // compute error
  std::vector<double> errorl(xc.size());
  for (unsigned int i=0;i<xc.size();++i)
    errorl[i]=fun(xc[i])-resultl[i];
  
  std::vector<double> errorn(xc.size());
  for (unsigned int i=0;i<xc.size();++i)
    errorn[i]=fun(xc[i])-resultn[i];
  
  std::vector<double> errorpl(xc.size());
   for (unsigned int i=0;i<xc.size();++i)
     errorpl[i]=fun(xc[i])-resultpl[i];

  cout<<"Xcoord   \t Lagrange \t  Newton  \t Piecewise  \t ErrLagr \t  ErrNewt \t ErrPlin"<<endl;
  for (unsigned int i=0;i<xc.size();++i){
    cout<<xc[i]<<"\t"<<resultl[i]<<"\t"<<resultn[i]<<"\t"<<resultpl[i]<<"\t"<<
      errorl[i]<<"\t"<<errorn[i]<<"\t"<<errorpl[i]<<endl;
  }
  
}
