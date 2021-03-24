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
  
  // useless : it is the default!!
  cout<<" Normal sort (case sensitive)"<<endl;
  std::sort(s.begin(),s.end(),CaseCompare());
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;


  cout<<" Sorted ignoring case"<<endl;
  //  std::sort(s.begin(),s.end(),NocaseCompare());
  std::sort(s.begin(),s.end(),Nocase_locale());
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;

  
  cout<< " Now using lambdas"<<endl;
  
  s[0]="Apple";
  s[1]="applejuice";
  s[2]="milk";
  s[3]="milKshake";
  s[4]="APPLE";
  s[5]="Milk";
  
  // useless : it is the default!!
  std::sort(s.begin(),s.end(),[](string const & a, string const &b){return a<b;});
  cout<<" Normal sort (case sensitive)"<<endl;
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;

  auto nocomp=[](std::string const & a, std::string const & b)
  {
    // Lexycografic comparison
    for(std::size_t i=0;i<std::min(a.size(),b.size()); ++i){
      if (std::toupper(a[i])!= std::toupper(b[i])) 
	return std::toupper(a[i])<std::toupper(b[i]);
    }
    return (a.size()<b.size());
  };
  std::sort(s.begin(),s.end(),nocomp);
  cout<<" Sorted ignoring case"<<endl;
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;



}
