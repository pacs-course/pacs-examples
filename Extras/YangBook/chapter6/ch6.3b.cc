// classes for vectors and matrices

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

void error(const char* v) {
  std::cout << v << "\n";
  std::exit(1);
}

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


// ***********  definitions of members in class Vtr.

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

Vtr operator+(const Vtr& v1, const Vtr & v2) {          // v=v1+v2
//  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
  Vtr sum = v1; // It would cause problem without copy constructor
  sum += v2;
  return sum;
}

Vtr operator-(const Vtr& v1, const Vtr& v2) { // v=v1-v2
//  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
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

// Matrix matrix

class Mtx {
private: 
  int nrows;                                   // number of rows in the matrix 
  int ncols;                                   // number of columns in  matrix 
  double** ets;                                // entries of the matrix
public: 
  Mtx(int n, int m, double**);                 // construct an n by m matrix
  Mtx(int n, int m, double d = 0);             // all entries equal d
  Mtx(const Mtx &);                            // copy constructor
  ~Mtx();                                      // destructor

  Mtx& operator=(const Mtx&);                  // overload =
  Mtx& operator+=(const Mtx&);                 // overload +=
  Mtx& operator-=(const Mtx&);                 // overload -=
  Vtr operator*(const Vtr&) const;             // matrix vector multiply
  double* operator[](int i) const { return ets[i]; }      // subscripting, row i

  // Implement plus as a member fcn. minus could also be so implemented.
  // But I choose to implement minus as a friend just to compare the difference 
  Mtx& operator+();                            // unary +, eg, mat1 = + mat2
  Mtx operator+(const Mtx&);                   // binary +, eg, m = m1 + m2
  friend Mtx operator-(const Mtx&);            // unary -, eg, m1 = -m2
  friend Mtx operator-(const Mtx&,const Mtx&); // binary -, eg, m = m1 - m2
};

Mtx::Mtx(int n, int m, double** dbp) {         // construct from double pointer
  nrows = n;
  ncols = m;
  ets = new double* [nrows]; 
  for (int i =  0; i < nrows; i++) {
    ets[i] = new double [ncols];          
    for (int j = 0; j < ncols; j++) ets[i][j] = dbp[i][j];
  }
}

Mtx::Mtx(int n, int m, double a) {              // construct from a double
  ets = new double* [nrows = n]; 
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols = m];
    for (int j = 0; j < ncols; j++) ets[i][j] = a;
  }
}

Mtx::Mtx(const Mtx & mat) {                      // copy constructor
  ets = new double* [nrows = mat.nrows]; 
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols = mat.ncols];
    for (int j = 0; j < ncols; j++) ets[i][j] = mat[i][j]; 
  }
}

Mtx::~Mtx(){                                    // destructor
  for (int i = 0; i< nrows; i++) delete[]  ets[i];
  delete[] ets;
}

Mtx& Mtx::operator=(const Mtx& mat) {           // copy assignment
  if (this != &mat) {
    if (nrows != mat.nrows || ncols != mat.ncols) error("bad matrix sizes");
    for (int i = 0; i < nrows; i++) 
      for (int j = 0; j < ncols; j++) ets[i][j]  = mat[i][j];
  }
  return *this;
}

Mtx& Mtx::operator+=(const Mtx&  mat) {         // add-assign
  if (nrows != mat.nrows || ncols != mat.ncols) error("bad matrix sizes");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++) ets[i][j] += mat[i][j];
  return *this;
}

Mtx& Mtx::operator-=(const Mtx&  mat) {         // subtract-assign
  if (nrows != mat.nrows || ncols != mat.ncols) error("bad matrix sizes");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++) ets[i][j] -= mat[i][j];
  return *this;
}

Mtx& Mtx::operator+() {                         // usage: mat1 = + mat2;
  return *this;
}

Mtx operator-(const Mtx &  mat) {               // usage: mat1 = - mat2;
  return Mtx(mat.nrows,mat.ncols) - mat;
}

Mtx Mtx::operator+(const Mtx & mat) {           // usage: m = m1 + m2
 Mtx sum = *this;                               // user-defined copy constructor
 sum += mat;                                    // is important here
 return sum;                                    // otherwise m1 would be changed
}

Vtr Mtx::operator*(const Vtr& v) const {        // matrix-vector multiply
  if (ncols != v.size()) error("matrix and vector sizes do not match");
  Vtr tm(nrows);
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++) tm[i] += ets[i][j]*v[j]; 
  return tm;
}

Mtx operator-(const Mtx& m1, const Mtx&  m2) {  // matrix subtract
 if(m1.nrows !=m2.nrows || m1.ncols !=m2.ncols) 
   error("bad matrix sizes");
 Mtx sum = m1;
 sum -= m2;
 return sum;
}

int main() {
  int k = 6;
  double** mt = new double* [k];
  for (int i = 0; i < k; i++ ) mt[i] =  new double [k];
  for (int i = 0; i < k; i++ ) 
    for (int j = 0; j < k; j++ ) mt[i][j] =  1;

  Mtx m1(k, k, mt);
  Mtx m2(k, k, 5);
  Mtx m3(k, k);
  for (int i = 0; i < k; i++ ) 
    for (int j = 0; j < k; j++ ) m3[i][j] =  2;

  m3 -= - m1 + m2;
  m3[1][1] = 3;

  std::cout << "m1[1][5] = " << m1[1][5]  <<  '\n';
  std::cout << "m2[1][5] = " << m2[1][5]  <<  '\n';
  std::cout << "m3[1][5] = " << m3[1][5]  <<  '\n';

  Vtr vv(k);
  Vtr v1(k, 5);
  Vtr v2(k, 1);
  for (int i = 0; i < k; i++ ) vv[i] = 1;
  //vv = m3*vv;
  vv = v1 + v2;

  std::cout << "v1[5] = " << v1[5]  <<  '\n';
  std::cout << "vv[5] = " << vv[5]  <<  '\n';

  {
  int k = 1000;
  Mtx A(k, k);
  Vtr x(k);
  Vtr y(k);
  Vtr z(k);
  for (int i = 0; i < k; i++ ) {
    x[i] = i;
    y[i] = i/(i + 30.0);
    for (int j = 0; j < k; j++ ) A[i][j] =  2.0/(i + j + 3.0);
  }

  time_t ck0 = time(0);
  for (int i = 0; i < 20; i++) {
    // z = A*x;                        // matrix vector multiply
    z = A*x + y;                       // matrix vector Gaxpy operation
  }

  std::cout << "seconds (straightforward) in Gaxpy = " 
       << (time(0) - ck0) << '\n';
  std::cout << "z[10] = " << z[10] << '\n'; 
  }

}
