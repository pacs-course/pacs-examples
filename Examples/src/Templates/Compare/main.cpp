#include<iostream>
#include "CompareString.hpp"
#include "policies.hpp"
int main(){
  using namespace std;
  string v1="Luca";
  string v2="luca";
  cout.setf(ios::boolalpha);
  cout<<"case sensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<equal<Ncomp>(v1,v2)<<endl; 
  cout<<"case insensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<equal<Nocase>(v1,v2)<<endl;

  cout<<" Now with the functor"<<std::endl;
  CompareString<Ncomp>  c1;
  CompareString<Nocase> c2;
  cout<<"case sensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<c1(v1,v2)<<endl; 
  cout<<"case insensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<c2(v1,v2)<<endl;

  

  
}
