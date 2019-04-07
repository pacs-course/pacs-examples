#include <vector>
#include <cctype> // for toupper()
#include <iostream>
#include <string> // for string
#include <algorithm> // for sort() and min()

// ************* compare: case sensitive and insensitive


/*
class Ncomp {                    // normal compare
public:
  bool operator () (std::string const & a, std::string const & b)
  {
    // I rely on the existing operator for strings
    return a< b;
  }
};
*/

class NoCaseCompare {                                 // compare by ignoring case 
public:
  bool operator () (std::string const & a, std::string const & b)
  {
    // Lexycografic comparison
    for(unsigned int i=0;i<std::min(a.size(),b.size()); ++i){
      char an=std::toupper(a[i]);
      char bn =std::toupper(b[i]);
      if (an != bn) return an<bn;
    }
    return (a.size()<b.size());
  }
};

class CaseCompare {                                 // Normal compare
public:
  bool operator () (std::string const & a, std::string const & b)
  {
    // Lexycografic comparison
    for(unsigned int i=0;i<std::min(a.size(),b.size()); ++i)
      if (a[i] != b[i]) return (a[i]<b[i]);
    return (a.size()<b.size());
  }
};

// In fact since < in this case defines a strict ordering relation ! (a<b || b<a) -> a==b we are authorised
// to use this functors also for containers. For instance
//
// set<string,Nocase> is a set with no repeated strings (ignoring case!).
//


int main(){
  using namespace std;
  
  // ************** compare
  vector<std::string> s(5);
  s[0]="Apple";
  s[1]="applejuice";
  s[2]="milk";
  s[3]="milKshake";
  s[4]="APPLE";

  vector<string> s1(s);

  std::sort(s.begin(),s.end(),CaseCompare());
  cout<<" Normal sort"<<endl;
  for (vector<std::string>::iterator i=s.begin();i<s.end();++i)
    cout<<*i<<" ";
  cout<<endl;
  
  std::sort(s1.begin(),s1.end(),NoCaseCompare());
  cout<<" Sorted ignoring case"<<endl;
  for (vector<std::string>::iterator i=s1.begin();i<s1.end();++i)
    cout<<*i<<" ";
  cout<<endl;
}
