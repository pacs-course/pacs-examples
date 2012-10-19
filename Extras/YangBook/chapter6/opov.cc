#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

void error(const char* v) {
  cout << v << "\n";
  std::exit(1);
}

class cmpx {      // class for complex numbers
private:
  double re;      // real part of a complex number
  double im;      // imaginal part of a complex number
public:
  cmpx(double x = 0, double y = 0) { re =x; im =y; } // another constructor
  cmpx& operator+=(cmpx);                      // operator +=, eg z1 +=  z2
  cmpx& operator-=(cmpx);                      // operator -=, eg z1 -=  z2
  cmpx operator++();                           // prefix,  z1 = ++z
  cmpx operator++(int);                        // postfix, z1 =  z++
  cmpx operator-();                            // operator -, eg z1 = - z2
  cmpx operator+();                            // operator +, eg z1 = + z2
  cmpx operator()(cmpx&);                      // operator +, eg z1 = + z2
//  friend cmpx operator+(cmpx);               // unary +, eg z1 = + z2
//  friend cmpx operator-(cmpx);               // unary -, eg z1 = - z2
  friend cmpx operator+(cmpx, cmpx);           // binary +, z=z1+z2
  friend cmpx operator-(cmpx, cmpx);           // binary -, z=z1-z2
  friend ostream& operator<<(ostream&, cmpx);  // operator <<
  friend ostream& operator>>(ostream&, cmpx&); // operator <<
};


class mtx;

class vtr {       
  int lenth;               // number of entries in the vector
  double* ets;             // entries of the vector
  friend class mtx;              // all functions of mtx becomes friends
public: 
  vtr(int, double*);       // costruct a vector with given entries
  vtr(int, double = 0);    // a vector with all entries same
  vtr(const vtr&);         // copy constructor
  ~vtr(){ delete[] ets; }  // destructor is defined inline

  vtr& operator=(const vtr&);       // overload = 
  vtr& operator+=(const vtr &);     // v += v2
  vtr& operator-=(const vtr &);     // v -= v2
  friend vtr operator+(const vtr&);  // unary operator, v = + v2
  friend vtr operator-(const vtr&);  // unary operator, v = - v2
  friend vtr operator+(const vtr&, const vtr&);      // v= v1+v2
  friend vtr operator-(const vtr&, const vtr&); 
  friend double dot(const vtr&, const vtr&);         // dot product
  double maxnorm() const;                            // maximum norm
  double euclidnorm() const;                         // L-2 norm
  friend vtr operator*(const double, const vtr&);    // vec-scalar x 
  friend vtr operator*(const vtr&, const double);    // vec-scalar x
  friend vtr operator*(const vtr&, const vtr&);
                                    // component-wise multiplication
  double& operator[](int i) const { return ets[i]; } // eg v[i] = 10;
  friend ostream& operator<<(ostream&, const vtr&);    
};


class mtx {
private: 
  int nrows;      // number of rows in the matrix 
  int ncols;      // number of columns in the matrix 
  double** ets;   // entries of the matrix
public: 
  mtx(int,int,double**);         // construct a matrix
  mtx(int,int,double = 0.0);     // construct a matrix
  mtx(const mtx &);              // copy constructor
  ~mtx(){                        // destructor
    for (int i = 0; i< nrows; i++) delete[]  ets[i];
    delete[] ets;
  }

  // implement + as a member fn. - could also be so implemented,
  // but I chose to implement - as a friend
  mtx& operator=(const mtx&);                  // overload =
  mtx& operator+=(const mtx&);
  mtx& operator-=(const mtx&);
  mtx& operator+();                            // unary +, eg mat1= +mat2
  mtx operator+(const mtx&);                   // binary +, m=m1+m2
  vtr operator*(const vtr&) const;             // matrix vector multiply
  double* operator[](int i) const { return ets[i]; }  // returns i-th row
  friend mtx operator-(const mtx&);            // unary -, m1 = -m2
  friend mtx operator-(const mtx&,const mtx&); // binary -
};

// ***********  definitions of cmpx
cmpx cmpx::operator++() {        // ++z
  (this->re)++;
  (this->im)++;
  return *this;
}

cmpx cmpx::operator++(int i) {   // z++
  cmpx temp = *this;
  (this->re)++;
  (this->im)++;
  return temp;
}


/*
inline cmpx operator+(cmpx z) {
  return z;
}

inline cmpx operator-(cmpx z) {
  return 0 - z;
}
*/

inline cmpx cmpx::operator+() {
  return *this;
}

inline cmpx cmpx::operator-() {
  return cmpx( - this->re, - this->im);
}

cmpx& cmpx::operator+=(cmpx z) {
  re += z.re;
  im += z.im;
  return *this;
}

cmpx& cmpx::operator-=(cmpx z) {
  re -= z.re;
  im -= z.im;
  return *this;
}

cmpx operator+(cmpx a, cmpx b) {     // binary +, eg z = z1 + z2
  cmpx temp = a;
  return a += b;  
}

cmpx operator-(cmpx a, cmpx b) {    // binary -, eg z = z1 - z2
  cmpx temp = a;
  return a -= b;  
}

ostream & operator<<(ostream& s, cmpx z) {
  s << "The real and imaginary parts are: " 
    << z.re << "  " << z.im;
  return s;
}


// ***********  definitions of members in class vtr.

vtr::vtr(int n, double* abd) {
  ets = new double [lenth =n]; 
  for (int i = 0; i < lenth; i++)  ets[i]= *(abd +i);
}

vtr::vtr(int n, double a) {
  ets = new double [lenth =n]; 
  for (int i = 0; i < lenth; i++)  ets[i] = a;
}

vtr::vtr(const vtr & v) {
  ets = new double [lenth = v.lenth]; 
  for (int i = 0; i < lenth; i++)  ets[i] = v[i]; 
}

vtr& vtr::operator=(const vtr& v) {
  if (this != &v) {
    if (lenth != v.lenth ) error("bad vector sizes");
    for (int i = 0; i < lenth; i++) ets[i] = v[i];
  }
  return *this;
}

vtr & vtr::operator+=(const vtr& v) {
  if (lenth != v.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) ets[i] += v[i];
  return *this;
}

vtr & vtr::operator-=(const vtr& v) {
  if (lenth != v.lenth ) error("bad vtor sizes");
  for (int i = 0; i < lenth; i++) ets[i] -= v[i];
  return *this;
}

vtr operator+(const vtr & v) {  // usage: v1 = + v2;
  return v;
}

vtr operator-(const vtr& v) {    // usage: v1 = - v2;
  return vtr(v.lenth) - v;
}

vtr operator+(const vtr& v1, const vtr & v2) {// v=v1+v2
  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
  vtr sum = v1; // It would cause problem without copy constructor
  sum += v2;
  return sum;
}

vtr operator-(const vtr& v1, const vtr& v2) { // v=v1-v2
  if (v1.lenth != v2.lenth ) error("bad vtor sizes");
  vtr sum = v1; // It would cause problem without copy constructor
  sum -= v2;
  return sum;
}

ostream&  operator<<(ostream& s, const vtr& v ) {
//  s << "elements of the vtor are: \n";
  for (int i =0; i < v.lenth; i++ ) {
    s << v[i] << "  ";
    if (i%10 == 9) s << "\n";
  }
  return s;
}

double vtr::euclidnorm() const{
  double norm = fabs(ets[0]);
  for (int i = 1; i < lenth; i++) {
    double vi = fabs(ets[i]);
    if (norm < 100) norm = sqrt(norm*norm + vi*vi);
    else {  // to avoid overflow for fn "sqrt" when norm is large
      double tm = vi/norm;
      norm *= sqrt(1.0 + tm*tm);
    }
  } 
  return norm;
}

double vtr::maxnorm() const {
  double norm = fabs(ets[0]);
  for (int i = 1; i < lenth; i++) 
    if (norm < fabs(ets[i])) norm = fabs(ets[i]);
  return norm;
}

vtr operator*(const double scalar, const vtr & v) {
  vtr tm(v.lenth);
  for (int i = 0; i < v.lenth; i++) tm[i] = scalar*v[i]; 
  return tm;
}

inline vtr operator*(const vtr & v, const double scalar) {
  return scalar*v; 
}

vtr operator*(const vtr & v1, const vtr & v2) {
  int sz = v1.lenth;
  if (sz != v2.lenth ) error("bad vtor sizes");
  vtr tm(sz);
  for (int i = 0; i < sz; i++) 
      tm[i] = v1[i]*v2[i]; 
  return tm;
}

vtr operator/(const vtr & v, const double scalar) {
  if (scalar == 0) error("division by zero in vector-scalar division");
  return (1.0/scalar)*v;
}

double dot(const vtr & v1, const vtr & v2) {
  int sz = v1.lenth;
  if (sz != v2.lenth ) error("bad vtor sizes");
  double tm = v1[0]*v2[0];
  for (int i = 1; i < sz; i++) tm += v1[i]*v2[i];
  return tm;
}


//********* mtx

mtx::mtx(int n, int m, double** dbp) {
  nrows = n;
  ncols = m;
  ets = new double* [nrows]; 
  if (!ets) error("no more space");
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols];          
    if (!ets[i]) error("no more space");
    for (int j = 0; j < ncols; j++) ets[i][j] = dbp[i][j];
  }
}

mtx::mtx(int n, int m, double a) {
  nrows = n;
  ncols = m;
  ets = new double* [nrows]; 
  if (!ets) error("no more space");
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols];
    if (!ets[i]) error("no more space");
    for (int j = 0; j < ncols; j++) ets[i][j] = a;
  }
}

mtx::mtx(const mtx & mat) {  // copy constructor
  nrows = mat.nrows;
  ncols = mat.ncols;
  ets = new double* [nrows]; 
  if (!ets) error("no more space");
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols];
    if (!ets[i]) error("no more space");
    for (int j = 0; j < ncols; j++) ets[i][j] =mat[i][j]; 
  }
}

mtx& mtx::operator=(const mtx& mat) {
  if (this != &mat) {
    if(nrows !=mat.nrows || ncols !=mat.ncols) error("bad matrix sizes");
    for (int i = 0; i < nrows; i++) 
      for (int j = 0; j < ncols; j++) ets[i][j]  = mat[i][j];
  }
  return *this;
}

mtx & mtx::operator+=(const mtx &  mat) {
  if(nrows !=mat.nrows || ncols !=mat.ncols) error("bad matrix sizes");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++)
      ets[i][j] += mat[i][j];

  return *this;
}

mtx & mtx::operator-=(const mtx &  mat) {
  if(nrows !=mat.nrows || ncols !=mat.ncols) error("bad matrix sizes");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++)
      ets[i][j] -= mat[i][j];

  return *this;
}

mtx & mtx::operator+() {    // usage: mat1 = + mat2;
  return *this;
}

mtx operator-(const mtx &  mat) {    // usage: mat1 = - mat2;
  return mtx(mat.nrows,mat.ncols) - mat;
}

mtx mtx::operator+(const mtx & mat) {  //m =m1+m2
 mtx sum = *this;
 sum += mat;
 return sum;
}

vtr mtx::operator*(const vtr& v) const {
  if (ncols != v.lenth ) 
    error("matrix and vector sizes do not match");
  vtr tm(nrows);
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++) 
      tm[i] += ets[i][j]*v[j]; 
  return tm;
}

mtx operator-(const mtx& m1, const mtx&  m2) {
 if(m1.nrows !=m2.nrows || m1.ncols !=m2.ncols) 
   error("bad matrix sizes");
 mtx sum = m1;
 sum -= m2;
 return sum;
}

int main() {
  cmpx a(1,1);
  cmpx b = a;
  cmpx c = a + b;
  c -=  b;
  cout << c << "\n";
  c = b;
  c = + b;
  cout << c << "\n";

  cmpx d;
  cout << d << "\n";
  cmpx e(3.0);
  cout << e << "\n";

  cmpx f = + e;
  cout << f << "\n";
  cmpx g = - e;
  cout << g << "\n";

cmpx aa = cmpx(5.0);
cmpx bb = ++aa;
cmpx cc = aa++;
cout << bb << "\n";
cout << cc << "\n";
cout << aa << "\n";

// ********** vectors

int k = 5;
double* v = new double [k];
for (int i = 0; i < k; i++ ) v[i] = i*i;
vtr v1(k, v);
vtr v2(k);

vtr v3 = - v1 + v2;
v3 += v1;
v3[2] = 2.0;
cout << v3 << "\n";

double** mt = new double* [k];
for (int i = 0; i < k; i++ ) mt[i] =  new double [k];
for (int i = 0; i < k; i++ ) 
  for (int j = 0; j < k; j++ ) mt[i][j] =  i*j;

mtx m1(k,k,mt);
mtx m2(k,k);
mtx m3(k,k, 5.0);
m3 += - m1 - m2;
m3[1][1] = 3;
vtr vv(k);
for (int i = 0; i < k; i++ ) vv[i] = i;
vv[1] = 2.0; 
vv = m3*vv;

cout << vv << "\n" ;

}
