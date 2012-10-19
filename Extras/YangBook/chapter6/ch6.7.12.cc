// efficient Gaxpy operations. classes for vectors and matrices

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

struct Gaxpy;                                        // forward declaration
struct Gax;                                          // forward declaration

class Vtr {                                          // a class for vector
  int lenth;                                         // number of entries 
  double* ets;                                       // entries of the vector
public: 
  Vtr(int, double*);                                 // costruct a vector 
  Vtr(int, double d = 0);                            // all entries equal d
  Vtr(const Vtr&);                                   // copy constructor
  Vtr(const Gax&);                                   // copy constructor
  Vtr(const Gaxpy&);                                 // copy constructor
  ~Vtr(){ delete[] ets; }                            // destructor 

  Vtr& operator=(const Vtr&);                        // copy assignment 
  Vtr& operator=(const Gax&);                        // overload =
  Vtr& operator=(const Gaxpy&);                      // overload =
  double& operator[](int i) const { return ets[i]; } // eg v[i] = 10;
  int size() const { return lenth; }                 // return length of vector
};

class Mtx {                                          // class for matrix 
  int nrows;                                         // number of rows 
  int ncols;                                         // number of columns 
  double** ets;                                      // entries of the matrix
public: 
  Mtx(int, int, double**);                           // construct a matrix
  Mtx(int, int, double = 0);                         // construct a matrix
  Mtx(const Mtx &);                                  // copy constructor
  ~Mtx();                                            // destructor
  int numrows() const { return nrows; }              // return number of rows
  int numcols() const { return ncols; }              // return number of columns
  double* operator[](int i) const { return ets[i]; } // returns row i
};

struct Gax {                                         // class for A x
  const Mtx& A;
  const Vtr& x;
  Gax(const Vtr& u, const Mtx& m) : A(m), x(u) { }   // constructor
};

inline Gax operator*(const Mtx& m, const Vtr& u) {   // overload *
  return Gax(u, m);                                  // for A*x
}

struct Gaxpy {                                       // class for A x and y
  const Mtx& A;
  const Vtr& x;
  const Vtr& y;
  Gaxpy(const Vtr& u, const Gax& g) : A(g.A), x(g.x), y(u) { }  // constructor
};

inline Gaxpy operator+(const Gax& g, const Vtr& u) {            // overload +
  return Gaxpy(u, g);                                           // for A*x + y
}

inline Gaxpy operator+(const Vtr& u, const Gax& g) {            // overload +
  return Gaxpy(u, g);                                           // for y + A*x 
}

Vtr::Vtr(int n, double* abd) {                       // define members in Vtr
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

Vtr::Vtr(const Gax& g) {
  ets = new double [lenth = g.A.numrows()]; 
  for (int i = 0; i < lenth; i++)  {
    ets[i] = 0; 
    for (int j = 0; j < g.x.size(); j++) ets[i] += g.A[i][j]*g.x[j];
  }
}

Vtr::Vtr(const Gaxpy& g) {
  ets = new double [lenth = g.A.numrows()]; 
  for (int i = 0; i < lenth; i++)  {
    ets[i] = 0; 
    for (int j = 0; j < g.x.size(); j++) ets[i] += g.A[i][j]*g.x[j];
    ets[i] += g.y[i]; 
  }
}

Vtr& Vtr::operator=(const Vtr& v) {
  if (this != &v) {
    for (int i = 0; i < lenth; i++) ets[i] = v[i];
  }
  return *this;
}

Vtr& Vtr::operator=(const Gax& g) {
  for (int i = 0; i < lenth; i++)  {
    ets[i] = 0; 
    for (int j = 0; j < g.x.size(); j++) ets[i] += g.A[i][j]*g.x[j];
  }
  return *this;
}

Vtr& Vtr::operator=(const Gaxpy& g) {
  for (int i = 0; i < lenth; i++)  {
    ets[i] = 0; 
    for (int j = 0; j < g.x.size(); j++) ets[i] += g.A[i][j]*g.x[j];
    ets[i] += g.y[i]; 
  }
  return *this;
  
}

Mtx::Mtx(int n, int m, double** dbp) {               // define members in Mtx 
  nrows = n;
  ncols = m;
  ets = new double* [nrows]; 
  for (int i =  0; i < nrows; i++) {
    ets[i] = new double [ncols];          
    for (int j = 0; j < ncols; j++) ets[i][j] = dbp[i][j];
  }
}

Mtx::Mtx(int n, int m, double a) { 
  ets = new double* [nrows = n]; 
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols = m];
    for (int j = 0; j < ncols; j++) ets[i][j] = a;
  }
}

Mtx::Mtx(const Mtx & mat) {                          // copy constructor
  ets = new double* [nrows = mat.nrows]; 
  for (int i =  0; i< nrows; i++) {
    ets[i] = new double [ncols = mat.ncols];
    for (int j = 0; j < ncols; j++) ets[i][j] = mat[i][j]; 
  }
}

Mtx::~Mtx(){                                         // destructor
  for (int i = 0; i< nrows; i++) delete[]  ets[i];
  delete[] ets;
}

int main() {
  int k = 500;
  Mtx A(k, k);
  Vtr x(k);
  Vtr y(k);
  Vtr z(k);
  for (int i = 0; i < k; i++ ) {
    x[i] = i;
    y[i] = i/(i +30.0);
    for (int j = 0; j < k; j++ ) A[i][j] =  2.0/(i + j + 3.0);
  }

  std::time_t ck0 = std::time(0);
  for (int i = 0; i < 100; i++) {
    z = A*x;                                   // matrix vector multiply
    z = A*x + y;                               // matrix vector Gaxpy operation
  }
  std::cout << "seconds (deferred evaluation) = " 
            << (time(0) - ck0)  << '\n';
  std::cout << "z[10] = "  << z[10] << '\n';
}
