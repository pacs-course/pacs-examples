#include <vector>
#include <cstdlib>
#include <cctype>
#include <iostream> 

// ************* compare: case sensitive and insensitive

template <class T, class S>
int compare(const std::vector<T>& v1, const std::vector<T>& v2) {
  for (unsigned int i = 0; i < v1.size() && i < v2.size(); i++ )
    if ( !S::eq(v1[i], v2[i]) ) return S::st(v1[i], v2[i]);
  return v2.size() - v1.size();
}

template <class T> class Ncomp {                    // normal compare
public:
  static int eq(T a, T b) { return a == b; }        // equal 
  static int st(T a, T b) { return a < b; }         // smaller than
};

class Nocase {                                 // compare by ignoring case 
public:
  static int eq(char a, char b) {              // equal, case-insensitive 
    return toupper(a) == toupper(b);
  }               
  static int st(char a, char b) {              // smaller than, case-insensitive
    return std::toupper(a) < std::toupper(b);
  } 
};

void f(std::vector<char> v1, std::vector<int> v2, 
       std::vector<char> v3, std::vector<int> v4) {
  compare<char, Ncomp<char> >(v1, v3);        // normal compare:case-sensitive
  compare<int, Ncomp<int> >(v2, v4);
  compare<char, Nocase>(v1, v3);              // case-insensitive compare
}

/*
template <class T, class S = Ncomp<T> >       // S is defaulted to Ncomp<T>
int compare2(const std::vector<T>& v1, const std::vector<T>& v2) {

  // define the function template as before
  for (int i = 0; i < v1.size() && i < v2.size(); i++ )
    if ( !S::eq(v1[i], v2[i]) ) return S::st(v1[i], v2[i]);
  return v2.size() - v1.size();
}
*/

int main(){
  int n = 3;

  // ************** compare
  std::vector<int> vv1(n);
  std::vector<int> vv2(n);
  for (int i=0; i <n;i++) { vv1[i] = 8; vv2[i] = 6; }

  int jj = compare<int, Ncomp<int> >(vv1, vv2);
  std::cout << " jj = " << jj << std::endl;

  std::vector<char> v3(n);
  std::vector<char> v4(n);
  v3[0]= 'A'; v3[1]= 'B'; v3[2]= 'C'; 
  v4[0]= 'a'; v4[1]= 'b'; v4[2]= 'c'; 

  jj = compare<char, Ncomp<char> >(v3, v4);
  jj = compare<char, Nocase>(v3, v4);
  //jj = compare2<char>(v3, v4);
  std::cout << " jj = " << jj << std::endl;


}
