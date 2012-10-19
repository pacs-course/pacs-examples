// class for efficient vectors

#include <iostream>
#include <ctime>
#include <cstdlib>

class Saxpy;

class Vtr {       
  int lenth;                            // number of entries in the vector
  double* ets;                          // entries of the vector
public: 
  Vtr(int, double*);                    // construct a vector with given entries
  Vtr(int, double = 0);                 // a vector with all entries same
  Vtr(const Vtr&);                      // copy constructor
  Vtr(const Saxpy&);                    // constructor from Saxpy
  ~Vtr(){ delete[] ets; }               // destructor is defined inline

  Vtr& operator=(const Vtr&);                        // overload assignment 
  Vtr& operator=(const Saxpy&);                      // overload assignment 

  int size() const { return lenth; }                 // return length of vector
  double& operator[](int i) const { return ets[i]; } // eg v[i] = 10;
};

class Sax {
  const double& a;                      // reference is used to avoid copying
  const Vtr& x;
public:
  Sax(const double& d, const Vtr& v) : a(d), x(v) { }       // constructor

/*
  operator Vtr() {                      // conversion operator
    Vtr tmp(x.size());
    for (int i = 0; i < x.size(); i++) tmp[i] = a*x[i]; 
    return tmp;
  }
*/

  friend Sax operator*(const double&, const Vtr&);
  friend class Saxpy;
};

inline Sax operator*(const double& d, const Vtr& v) {
  return Sax(d,v);
}

class Saxpy {
  const double& a;                      // reference is used to avoid copying
  const Vtr& x;
  const Vtr& y;
public:
  Saxpy(const Sax& s, const Vtr& u) : a(s.a), x(s.x), y(u) { }   // constructor
//  operator Vtr();                       // conversion operator
  friend class Vtr;                       // all members of Vtr become friend
  friend Saxpy operator+(const Sax&, const Vtr&);
};

/*
Saxpy::operator Vtr() {                   // conversion operator
  Vtr tmp(x.size());
  for (int i = 0; i < x.size(); i++) tmp[i] = a*x[i] + y[i]; 
  return tmp;
}
*/

inline Saxpy operator+(const Sax& s, const Vtr& u) {
  return Saxpy(s, u);
}

inline Saxpy operator+(const Vtr& u, const Sax& s) {
  return Saxpy(s, u);
}

void error(const char* v) {
  std::cout << v << "\n";
  std::exit(1);
}

Vtr::Vtr(int n, double* abd) {
  ets = new double [lenth =n]; 
  for (int i = 0; i < lenth; i++)  ets[i]= *(abd +i);
}

Vtr::Vtr(int n, double a) {
  ets = new double [lenth =n]; 
  for (int i = 0; i < lenth; i++)  ets[i] = a;
}

Vtr::Vtr(const Vtr& v) {
  ets = new double [lenth = v.lenth]; 
  for (int i = 0; i < lenth; i++)  ets[i] = v[i]; 
}

Vtr::Vtr(const Saxpy& sp) {
 ets = new double [lenth = sp.x.size()];
 for (int i = 0; i < lenth; i++) ets[i] = sp.a*sp.x[i] + sp.y[i]; 
}

Vtr& Vtr::operator=(const Saxpy& sp) {
  for (int i = 0; i < lenth; i++) ets[i] = sp.a*sp.x[i] + sp.y[i];
  return *this;
}


Vtr& Vtr::operator=(const Vtr& v) {
  if (this != &v) {
    if (lenth != v.lenth ) error("bad vector sizes");
    for (int i = 0; i < lenth; i++) ets[i] = v[i];
  }
  return *this;
}

inline void saxpy(double a, const Vtr& x, const Vtr& y, Vtr& z) {
  for (int i = 0; i < x.size(); i++) z[i] = a*x[i] + y[i];
}

inline void saxpy2(double a, const Vtr& x, const Vtr& y, Vtr& z) {
  int size = x.size();
  for (int i = 0; i < size; i++) z[i] = a*x[i] + y[i];
}

int main() {
  int k = 50000;
  int m = 1000;
  double* p = new double [k];
  for (int i = 0; i < k; i++ ) p[i] = i*i + 10;
  Vtr v(k, p);
  Vtr w(k, 5);
  Vtr u(k);
  //v =  u + 3.14*w;

  std::time_t t1 = std::time(0);
  for (int i = 0; i < m; i++) {
    u =  3*v + w;
  }
  std::time_t t2 = std::time(0);
  std::cout << "time in smart op ov is "  << std::difftime(t2,t1) << "\n";

  for (int i = 0; i < m; i++) {
    saxpy(3, v, w, u);
  }
  time_t t3 = std::time(0);
  std::cout << "time in traditional way is "  << std::difftime(t3,t2) << "\n";

  for (int i = 0; i < m; i++) {
    saxpy2(3, v, w, u);
  }
  std::time_t t4 = std::time(0);
  std::cout << "time in traditional way 2 is "  << std::difftime(t4,t3) << "\n";
}
