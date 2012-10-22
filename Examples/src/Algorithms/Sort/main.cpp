#include <iostream>
#include <vector>
#include "compare.hpp"

int main(){
  using namespace std;
  
  // ************** compare
  vector<std::string> s(6);
  s[0]="Apple";
  s[1]="applejuice";
  s[2]="milk";
  s[3]="milKshake";
  s[4]="APPLE";
  s[5]="Milk";
  
  std::sort(s.begin(),s.end(),Nocase());
  cout<<" Sorted ignoring case"<<endl;
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;

  std::sort(s.begin(),s.end(),Ncomp());
  cout<<" Normal sort (case sensitive)"<<endl;
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;
  
}
