#include<iostream>
#include "CompareString.hpp"
#include "policies.hpp"
#include <unordered_set>
#include <vector>
int main(){
  using namespace std;
  string v1="Luca";
  string v2="luca";
  cout.setf(ios::boolalpha);
  cout<<"case sensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<equal<CaseCompare>(v1,v2)<<endl; 
  cout<<"case insensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<equal<NoCaseCompare>(v1,v2)<<endl;

  cout<<" Now with the functor"<<std::endl;
  CompareString<CaseCompare>  c1;
  CompareString<NoCaseCompare> c2;
  cout<<"case sensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<c1(v1,v2)<<endl; 
  cout<<"case insensitive comparison"<<endl;
  cout<<v1<< " is equal to "<<v2<<" "<<c2(v1,v2)<<endl;
// I use the different equivalence relations for unordered sets
  std::vector<string> values={"Luca","luca","Marco","marco"};
  std::unordered_set<string,std::hash<string>,CompareString<CaseCompare>> caseCompare{values.begin(),values.end()};
  std::unordered_set<string,HashNoCase,CompareString<NoCaseCompare>> noCaseCompare{values.begin(),values.end()};
  caseCompare.insert("Mary");
  caseCompare.insert("MARY");
  auto m1=noCaseCompare.insert("Mary");
  auto m2=noCaseCompare.insert("MARY");
  std::cout<<"has MARY been inserted? "<<m2.second<<std::endl;
  std::cout<<*(m1.first)<<" and "<<*(m2.first)<< " are the same? "<<noCaseCompare.key_eq()(*(m1.first),*(m2.first))<<std::endl;

  std::cout<<"Content of caseCompare set (size="<<caseCompare.size()<<"):\n";
  for (auto const & v: caseCompare) std::cout<< v<<" ";
  std::cout<<"\nContent of noCaseCompare set (size="<<noCaseCompare.size()<<"):\n";
  for (auto const & v: noCaseCompare) std::cout<< v<<" ";
  std::cout<<std::endl;
  

}
