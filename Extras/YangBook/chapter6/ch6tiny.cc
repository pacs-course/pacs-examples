#include <iostream>
#include <cstdlib>
class tiny {
  char v;
  void assign (int i) {                                   // for range checking
    if (i & ~0xf) { std::cout << "range error\n"; std::exit (1); }
    v = i;
  }
public:
  tiny(int i) { assign(i); }                              // constructor
  tiny& operator=(int i) { assign(i); return *this; }     // assignment
  operator int() const { return v; }                      // conversion operator
};

inline tiny operator+(tiny t, tiny s) {
  return tiny(int(t) + int(s));                                 
}

void f(tiny t, int i) {

  // error, ambiguous: int(t) + i or  t + tiny(i) ?
  //std::cout << t + i << '\n'; 
  // OK: explicit conversion
  std::cout << t + tiny(i) << '\n'; 
}

void ff(tiny t, int i) {
  std::cout << t + tiny(i) << '\n'; 
}

void fff(tiny t, int i) {
  std::cout << int(t) + i << '\n'; 
}

int main() {
  tiny t = 12;       // int is assigned to tiny by tiny& tiney::operator=(int)
  int i  = t;        // tiny is converted to int by tiny::operator int() 
//tiny tt = 16;      // range error occurs, program exits.

  std::cout << "i = " << i << '\n';
  std::cout << "tt = " << int(t) << '\n';

  f(t, 12);
  ff(t, 12);
  fff(t, 12);
}

