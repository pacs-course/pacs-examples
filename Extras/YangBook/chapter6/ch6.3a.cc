// class for vectors

#include <iostream>
#include <cmath>
#include <ctime>
#include<cstdlib>

class Vtr {       
  int lenth;               // number of entries in the vector
  double* ets;             // entries of the vector
public: 
  Vtr(int, double*);       // costruct a vector with given entries
  Vtr(int, double = 0);    // a vector with all entries same
  Vtr(const Vtr&);         // copy constructor
  ~Vtr(){ delete[] ets; }  // destructor is defined inline

  Vtr& operator=(const Vtr&);                        // copy assignment 
  Vtr& operator+=(const Vtr &);                      // v += v2
  Vtr& operator-=(const Vtr &);                      // v -= v2
  double& operator[](int i) const { return ets[i]; } // eg v[i] = 10;
  double maxnorm() const;                            // maximum norm
  double twonorm() const;                            // L-2 norm
  int size() const { return lenth; }                 // return length of vector
  friend Vtr operator+(const Vtr&);                  // unary +, v = + v2
  friend Vtr operator-(const Vtr&);                  // unary -, v = - v2
  friend Vtr operator+(const Vtr&, const Vtr&);      // binary +, v = v1+v2
  friend Vtr operator-(const Vtr&, const Vtr&);      // binary -, v = v1-v2
  friend double dot(const Vtr&, const Vtr&);         // dot product
  friend Vtr operator*(const double, const Vtr&);    // vec-scalar x 
  friend Vtr operator*(const Vtr&, const double);    // vec-scalar x
  friend Vtr operator*(const Vtr&, const Vtr&);      // component-wise multiply
  friend std::ostream& operator<<(std::ostream&, const Vtr&); // output operator
};

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

Vtr::Vtr(const Vtr & v) {
  ets = new double [lenth = v.lenth]; 
  for (int i = 0; i < lenth; i++)  ets[i] = v[i]; 
}

Vtr& Vtr::operator=(const Vtr& v) {
  if (this != &v) {
//    if (lenth != v.lenth ) error("bad vector sizes");
    for (int i = 0; i < lenth; i++) ets[i] = v[i];
  }
  return *this;
}

Vtr & Vtr::operator+=(const Vtr& v) {
  if (lenth != v.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) ets[i] += v[i];
  return *this;
}

Vtr & Vtr::operator-=(const Vtr& v) {
  if (lenth != v.lenth ) error("bad vtor sizes");
  for (int i = 0; i < lenth; i++) ets[i] -= v[i];
  return *this;
}

Vtr operator+(const Vtr & v) {  // usage: v1 = + v2;
  return v;
}

Vtr operator-(const Vtr& v) {    // usage: v1 = - v2;
  return Vtr(v.lenth) - v;
}

Vtr operator+(const Vtr& v1, const Vtr & v2) {// v=v1+v2
//  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
  Vtr sum = v1; // It would cause problem without copy constructor
  sum += v2;
  return sum;
}

Vtr operator-(const Vtr& v1, const Vtr& v2) { // v=v1-v2
  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
  Vtr sum = v1; // It would cause problem without copy constructor
  sum -= v2;
  return sum;
}

std::ostream&  operator<<(std::ostream& s, const Vtr& v ) {
  for (int i =0; i < v.lenth; i++ ) {
    s << v[i] << "  ";
    if (i%10 == 9) s << "\n";
  }
  return s;
}

double Vtr::twonorm() const{
  double norm = std::abs(ets[0]);
  for (int i = 1; i < lenth; i++) {
    double vi = std::abs(ets[i]);
    if (norm < 100) norm = std::sqrt(norm*norm + vi*vi);
    else {  // to avoid overflow for fn "sqrt" when norm is large
      double tm = vi/norm;
      norm *= std::sqrt(1.0 + tm*tm);
    }
  } 
  return norm;
}

double Vtr::maxnorm() const {
  double norm = std::abs(ets[0]);
  for (int i = 1; i < lenth; i++) 
    if (norm < std::abs(ets[i])) norm = std::abs(ets[i]);
  return norm;
}

Vtr operator*(const double scalar, const Vtr & v) {
  Vtr tm(v.lenth);
  for (int i = 0; i < v.lenth; i++) tm[i] = scalar*v[i]; 
  return tm;
}

inline Vtr operator*(const Vtr & v, const double scalar) {
  return scalar*v; 
}

Vtr operator*(const Vtr & v1, const Vtr & v2) {
  int sz = v1.lenth;
  if (sz != v2.lenth ) error("bad vtor sizes");
  Vtr tm(sz);
  for (int i = 0; i < sz; i++) 
      tm[i] = v1[i]*v2[i]; 
  return tm;
}

Vtr operator/(const Vtr & v, const double scalar) {
  if (scalar == 0) error("division by zero in vector-scalar division");
  return (1.0/scalar)*v;
}

double dot(const Vtr & v1, const Vtr & v2) {
  int sz = v1.lenth;
  if (sz != v2.lenth ) error("bad vtor sizes");
  double tm = v1[0]*v2[0];
  for (int i = 1; i < sz; i++) tm += v1[i]*v2[i];
  return tm;
}


int main() {

  // ********** vectors

  int k = 50000;
  int m = 1000;

  double* v = new double [k];
  for (int i = 0; i < k; i++ ) v[i] = i*i + 10;
  Vtr v1(k, v);
  Vtr v2(k, 5);
  Vtr v3(k);

  //v2 = 5*v1;

  time_t t1 = time(0);
  for (int i = 0; i < m; i++) {
    v3 = 3*v1 + v2;
  }
  time_t t2 = time(0);
  std::cout << "time in Vtr (straightforward op ov) is " 
            << std::difftime(t2, t1) << '\n';

  /*
  v3 += v1;
  v3[2] = 2.0;
  std::cout << v3 << "\n";
  */

}
