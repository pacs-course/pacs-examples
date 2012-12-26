#include "sum.hpp"
#include "Vcr.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
int main(){
  using namespace std;
  vector<int>v1(10,1);
  vector<double>v2(5,2.0);
  cout<<"Using sum1"<<endl;
  cout<<sum1(v1)<<endl;
  cout<<sum1(v2)<<endl;
  cout<<"Using sum2"<<endl;
  cout<<sum2(v1)<<endl;
  cout<<sum2(v2)<<endl;
  vector<double *> v3(5);
  for(unsigned int i=0;i!=v3.size();++i)v3[i]=&(v2[i]);
  cout<<"Working with pointers"<<endl;
  cout<<sum2(v3)<<endl;
  cout<<"Working with Vcr"<<endl;
  Vcr<double> v4(5,new double[5]);
  // copy works also with pointers!
  std::copy(v2.begin(),v2.end(),&(v4[0]));
  cout<<sum2(v4)<<endl;
  cout<<"Using iterators (pointers in fact!)"<<endl;
  cout<<sum3(&(v4[0]),&(v4[5]))<<endl;
  
}
