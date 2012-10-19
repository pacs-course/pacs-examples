#include <cstdlib>
#include <cmath>
#include <iostream> 
#include <complex>
#include <algorithm>

class Vcr {       
  int lenth;                                     // number of entries 
  double* vr;                                    // entries of the vector
public: 
  Vcr(int n, double* t);                         // constructor from t
  explicit Vcr(int n, double t = 0);             // constructor, all entries =t
  Vcr(const Vcr&);                               // copy constructor
  ~Vcr(){ delete[] vr; }                         // destructor

  int size() const { return lenth; }             // return size of vector
  double& operator[](int i) const { return vr[i]; }     // v[i] = 10;
  Vcr& operator=(const Vcr&);                    // overload =, v = v2
  Vcr& operator+=(const Vcr&);                   // v += v2
  Vcr& operator-=(const Vcr&);                   // v -= v2
  double twonorm() const;                        // L-2 norm

  friend Vcr operator+(const Vcr&, const Vcr&);  // v= v1 + v2
  friend Vcr operator-(const Vcr&, const Vcr&);  // v= v1 - v2
  friend double dot(const Vcr&, const Vcr&);     // dot product
  friend Vcr operator*(double, const Vcr&);      // scalar-vector multiply 
};


class Mtx  {                                      // square matrix
private: 
  int dimn;                                       // dimension of matrix
  double** mx;                                    // entries of the matrix
  Mtx& operator=(const Mtx&);                     // make = private
  Mtx(const Mtx &);                               // make copying private
public: 
  Mtx(int n, double**);                           // n: # of rows = # of columns
  Mtx(int n, double t = 0);                       // all entries are set to t
  ~Mtx(){                                         // destructor
    for (int i = 0; i< dimn; i++) delete[]  mx[i];
    delete[] mx;
  }
  Vcr operator*(const Vcr&) const;                // matrix-vector multiply
  double* operator[](int i) const { return mx[i]; }     // returns row i

  int CG(Vcr& x, const Vcr& b, double& eps, int& iter);  
      // Conjugate gradient method for Ax=b. A: sym pos def
      // x:  on entry:  initial guess; on return: approximate solution
      // b:  right side vector 
      // eps: on entry:  stopping criterion, epsilon
      //      on return: absolute residual in two-norm for approximate solution
      // iter: on entry:  max number of iterations allowed; 
      //       on return: actual number of iterations taken.
      // it returns 0 for sucessful return and 1 for breakdowns
};

//*** report error and exit with return value 1.
void error(const char* t) {
  std::cout << t << ". program exited." << "\n";
  exit(1);
}

// *** definitions for members of class Mtx

Mtx::Mtx(int n, double** dbp) {
  dimn = n;
  mx = new double* [dimn]; 
  for (int i =  0; i< dimn; i++) {
    mx[i] = new double [dimn];          
    for (int j = 0; j < dimn; j++) mx[i][j] = dbp[i][j];
  }
}

Mtx::Mtx(int n, double a) {
  dimn = n;
  mx = new double* [dimn]; 
  for (int i =  0; i< dimn; i++) {
    mx[i] = new double [dimn];
    for (int j = 0; j < dimn; j++) mx[i][j] = a;
  }
}

Vcr Mtx::operator*(const Vcr & vec) const {
  if (dimn!= vec.size()) 
    error("matrix and vector sizes do not match in Mtx::operator*()");
  Vcr tm(dimn);
  for (int i = 0; i < dimn; i++) 
    for (int j = 0; j < dimn; j++) tm[i] += mx[i][j]*vec[j]; 
  return tm;
}

int Mtx::CG(Vcr& x, const Vcr& b, double& eps, int& iter) {
  if (dimn!= b.size()) {
    std::cout << "matrix and vector sizes do not match\n"; 
    std::exit(1);
  }
  const int maxiter = iter;
  Vcr r = b - (*this)*x;                      // initial residual 
  Vcr p = r;                                  // p: search direction
  double zr = dot(r,r);                       // inner prod of r and r
  const double stp = eps*b.twonorm();         // stopping criterion

  if (r.twonorm() == 0) {                     // if intial guess is true soln, 
    eps = 0.0;                                // return. Otherwise division by 
    iter = 0;                                 // zero would occur.
    return 0; 
  }

  for (iter = 0; iter < maxiter; iter++) {    // main loop of CG method
    Vcr mp = (*this)*p;                       // matrix-vector multiply
    double alpha = zr/dot(mp,p);              // division by 0 only when r is 0
    x += alpha*p;                             // update the iterative soln
    r -= alpha*mp;                            // update residual 
    if (r.twonorm() <= stp) break;            // stop if convergence achieved
    double zrold = zr;
    zr = dot(r,r);                            // another of two inner products
    p = r + (zr/zrold)*p;                     // zrold = 0 only when r is 0
  } 

  eps = r.twonorm();
  if (iter == maxiter) return 1;
  else return 0;
}                                             // end CG()


// *** definitions for members in class Vcr.

Vcr::Vcr(int n, double* abd) {
  vr = new double [lenth = n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd +i);
}

Vcr::Vcr(int n, double a) {
  vr = new double [lenth = n]; 
  for (int i = 0; i < lenth; i++)  vr[i] = a;
}

Vcr::Vcr(const Vcr & vec) {
  vr = new double [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec.vr[i]; 
}

Vcr& Vcr::operator=(const Vcr& vec) {
  if (this != &vec) {
    if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator=()");
    for (int i = 0; i < lenth; i++) vr[i] = vec.vr[i];
  }
  return *this;
}

Vcr & Vcr::operator+=(const Vcr& vec) {
  if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator+=()");
  for (int i = 0; i < lenth; i++) vr[i] += vec.vr[i];
  return *this;
}

Vcr & Vcr::operator-=(const Vcr& vec) {
  if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator-=()");
  for (int i = 0; i < lenth; i++) vr[i] -= vec.vr[i];
  return *this;
}

Vcr operator+(const Vcr& v1, const Vcr& v2) {         // v=v1+v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vecor addition");
  Vcr sum = v1; 
  sum += v2;
  return sum;
}

Vcr operator-(const Vcr& v1, const Vcr & v2) {         // v=v1-v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vector subtraction");
  Vcr sum = v1;           // It would cause problem without copy constructor
  sum -= v2;
  return sum;
}

double Vcr::twonorm() const {
  double nm = std::abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    double vi = std::abs(vr[i]);
    if (nm < 100) nm = std::sqrt(nm*nm + vi*vi);
    else {                  // to avoid overflow for fn "sqrt" when nm is large
      double tm = vi/nm;
      nm *= std::sqrt(1.0 + tm*tm);
    }
  } 
  return nm;
}

Vcr operator*(double scalar, const Vcr & vec) {
  Vcr tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = scalar*vec[i]; 
  return tm;
}

double dot(const Vcr & v1, const Vcr & v2) {
  if (v1.lenth != v2.lenth ) error("bad vector sizes in dot product");
  double tm = v1[0]*v2[0];
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*v2[i];
  return tm;
}

int main() {

  int n = 300;
  Mtx a(n);                                 // a matrix of n by n
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) a[i][j] = 1/(i + j + 1.0);

  Vcr b(n) ;                                // right hand vector of size n
  Vcr x(n);                                 // initial guess amd solution vector
  for (int i = 0; i < n; i++) b[i] = 1/(i + 3.14);


  int iter = 300;
  double eps = 1.0e-9;

  int ret =  a.CG(x, a*b, eps, iter);       // call conjugate gradient algorithm
  if (ret == 0) std::cout << "CG returned successfully\n";
  std::cout << iter << " iterations are used in conjugate gradient method.\n";
  std::cout << "Residual = " << eps << ".  ";
  std::cout << "Two-norm of exact error vector = " << (x - b).twonorm() << '\n';
}
