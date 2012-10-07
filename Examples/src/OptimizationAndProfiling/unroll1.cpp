#include <iostream>
#include <cmath> // for sqrt
#include "chrono.hpp"
int main()
{
  Timings::Chrono myclock;
  double v[10];
  double w[10];
  double s[10];
  int n;
  for (int i=1; i<5;++i){v[i]=i;w[i]=-2*i;}
  std::cout<<" Give me number of external loops"<<std::endl;
  
  std::cin>>n;
  myclock.start();
  for (int j=0;j<n;++j){
    /// Internal loop 
  for (int i=1; i<10; ++i){
    s[i]=v[i]*v[i]-w[i]-5;
  };
  for (int i=1; i<10; ++i){
    v[i]+=std::min(std::max(w[i]*w[i]-s[i],10.0),100.0);
    w[i]=s[i]-v[i];
  };
  double sum=v[0]+v[5];
  }
  myclock.stop();
  std::cout<<myclock<<std::endl;
  // to avod complain about unused variables
  std::cout<<"The value of s[3] is"<< s[3]<<std::endl<<std::endl;
  
  
}
