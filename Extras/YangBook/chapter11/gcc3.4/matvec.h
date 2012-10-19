/******************** matvec.h ***************************
 * This is a template library header file for manipulating and solving
 * matrices and vectors, whose entries can be int, float, double,
 * long double, and complex<double>, etc. This file contains

    ~ definitions for template classes 
        * AbsMtx<T>:     an abstract matrix, 
        * Vcr<T>:        a vector class, 
        * FullMtx<T>:    matrix in full storage, 
        * SparseMtx<T>:  sparse matrix in compressed sparse row storage, 
        * BandMtx<T>:    banded matrix, 
        * SSMtx<T>:      symmetric sparse matrix, 
    ~ declaration of complete instantiation Vcr< complex<double> >

  In particular, it contains

    ~ basic operations (eg add, multiply) on matrices and vectors
    ~ norms and dot products for vectors of real and complex entries
    ~ preconditioned conjugate gradient method CG()
    ~ preconditioned restarted GMRES(m) for general nonsingular matrices, 
    ~ Gauss elimination without pivoting GaussElim() for full & band matrices
    ~ Gauss elim with partial pivoting GaussElimPP() for full & band matrices
    ~ Gauss elim with complete pivoting GaussElimCP() for full matrices

 Functions CG(), GMRES(), GaussElim(), GaussElimPP(), GaussElimCP() 
 apply to linear systems of real and complex entries. 
 However, GaussElim() and CG() are guaranted to work only for 
 Hermitian positive definite matrices and GaussElim() 
 also for diagonally dominated matrices. 

 *                        written by Daoqi Yang
 *                        last modified 2001-08-13
 **********************************************************/

#ifndef MATVEC_H
#define MATVEC_H

#include <cstdlib>
#include <cmath>
#include <iostream> 
#include <complex>
#include <algorithm>

using namespace std;

template<class T> class Vcr;                   // forward declaration

class Vcr< complex<double> >;                  // forward declaration

  
template<class T> class AbsMtx{               // base matrix, an abstract class
protected:
  int nrows;                                   // number of rows in the matrix 
  virtual Vcr<T> preconding(const Vcr<T>& r, int i = 0) const = 0; 
      // solve P z = r with preconditiner P and vector r, it returns z
      // i = 0 if no preconditioning, i=1 if diag prec, i = 2 if SSOR  prec
public: 
  virtual Vcr<T> operator*(const Vcr<T> &) const = 0;  // matrix vector multiply
  AbsMtx<T>():nrows(0){};
  virtual ~AbsMtx<T>(){};
  int CG(Vcr<T>& x, const Vcr<T>& b, double& eps, int& iter, int pn);  
      // preconditioned Conjugate gradient method for Ax=b. A: sym pos def
      // x:  on entry:  initial guess; on return: approximate solution
      // b:  right side vector 
      // eps: on entry:  stopping criterion, epsilon
      //      on return: absolute residual in two-norm for approximate solution
      // iter: on entry:  max number of iterations allowed; 
      //       on return: actual number of iterations taken.
      // pn: =0 if no preconditioner, =1 if diag precr, =2 if SSOR precr
      // it returns 0 for sucessful return and 1 for breakdowns

  int GMRES(Vcr<T>& x, const Vcr<T>& b, double& eps, int& iter, int pn, int m);
      // restarted GMRES for solve Ax=b, where A may be non-symmetric
      // x: on entry: initial guess; on return: approximate solution
      // b: right side vector
      // eps: on input, stopping criterion,
      //      on return, absolute two-norm residual upon program termination
      // iter: on input, max number of iterations allowed; 
      //       on return, actual number of iterations taken.
      // pn: =0 if no preconditioner is used, =1 if diag precr, =2 if SSOR precr
      // m: number of iterations for restarting GMRES
      // it returns 0 for sucessful return and 1 for breakdowns
};

template<class T> class FullMtx: public AbsMtx<T> {        // full matrix
  using AbsMtx<T>::nrows;
private: 
  int ncols;                                    // # of columns in the matrix 
  T** mx;                                       // entries of the matrix
  Vcr<T> preconding(const Vcr<T>&, int i = 0) const; 
      // i = 0: no preconditioner, i = 1: diag precr, i = 2: SSOR precr
public: 
  FullMtx(int n, int m, T**);         // n: # of rows, m: # of columns
  FullMtx(int n, int m, T t = 0);     // all entries are set to t
  FullMtx(const FullMtx&);            // copy constructor
  ~FullMtx() {                         // destructor
    for (int i = 0; i< nrows; i++) delete[]  mx[i];
    delete[] mx;
  }

  // implement + as a member fcn and  implement - as a friend
  FullMtx& operator=(const FullMtx&);           // overload =
  FullMtx& operator+=(const FullMtx&);          // overload +=
  FullMtx& operator-=(const FullMtx&);          // overload -=
  FullMtx& operator+();                         // unary +, m1 = + m2;
  FullMtx operator+(const FullMtx&);            // binary +, m = m1 + m2
  Vcr<T> operator*(const Vcr<T>&) const;        // matrix-vector multiply
  T* operator[](int i) const { return mx[i]; }  // returns i-th row

  template <class S> friend 
  FullMtx<S> operator-(const FullMtx<S>&);      // unary -, m1 = -m2;

  template <class S> friend FullMtx<S> 
  operator-(const FullMtx<S>&, const FullMtx<S>&);      // binary -, m=m1-m2

  template <class S> friend 
  ostream& operator<<(ostream&, const FullMtx<S>&);     // overload << 

  void GaussElim(Vcr<T>&) const;        // Gauss elimination
  void GaussElimPP(Vcr<T>&) const;      // Gauss Elim with partial pivot
  void GaussElimPP2(Vcr<T>&) const;     // Gauss Elim with partial pivot
  void GaussElimCP(Vcr<T>&) const;      // Gauss Elim with complte pivot
};

template<class T>
class SSMtx: public AbsMtx<T> {                 // Symmetric Sparse matrix
  using AbsMtx<T>::nrows;
private: 
  int lenth;        // # of diag entries and nonzero off-diag entries 
                    // in the lower triangular part of the matrix, 
  T* sra;           // store diag entries and non-zero off-diagonal entries 
  int *dgn;         // position of matrix diagonals as stored in sra 
  int *clm;         // columns in original matrix for entries in sra 
  Vcr<T> preconding(const Vcr<T> &, int = 0) const; 
      // solve preconditioning matrix P z = r with preconditioner P
      // int = 0: no preconditioning, = 1: diag precond, = 2: SSOR precond
public: 
  SSMtx(int n, int m, T* t, int* c, int* i);     // n = nrows, m = lenth
      // t: 1D array storing diag entries and nonzero off-diag entries
      // c: colunms (in the original matrix) of entries of the 1D array t 
      // i: index of diags in the 1D array.
  SSMtx(int n, int m);                           // initialize a zero SSMtx
      // n: number of rows of the Sym Sparse matrix = number of columns
      // m: # of diag entries & nonzero off-diag entries in lower triangular part
  SSMtx(const SSMtx &);                          // copy constructor
  ~SSMtx(){ delete[] sra; delete[] dgn; delete[] clm; }
	
  SSMtx& operator=(const SSMtx & );              // overload =
  T& operator[](int i) const { return sra[i]; }
  int& getdgn(int i) const { return dgn[i]; }
  int& getclm(int i) const { return clm[i]; }
  Vcr<T> operator*(const Vcr<T>&) const;
};

template<class T>                            // sparse matrix
class SparseMtx: public AbsMtx<T> {          // compressed sparse row format
  using AbsMtx<T>::nrows;
  
private: 
  int lenth;             // # of nonzero entries of the original matrix 
  T *sra;                // array for storing the non-zero entries 
  int *clm;              // column indexes in matrix of the entries in sra 
  int *fnz;              // position in sra of first nonzero entires of each row
  Vcr<T> preconding(const Vcr<T>&, int i = 0) const; 
      // i = 0: no preconditioner, i = 1: diag precr, i = 2: SSOR precr
public: 
  SparseMtx(int n, int m, T* t, int* c, int* f);     
      // n: number of rows (and columns) of the original matrix
      // m: length of array sra for nonzero entries.
      // t: nonzero entries of the original matrix
      // c: colunm indexes (in the original matrix) of entries in sra 
      // f: index in sra of first nonzero entry in each row

  SparseMtx(int n, int m);                       // intialize all entris to zero
      // n: number of rows (and columns). m: number of nonzero entries. 

  SparseMtx(const SparseMtx&);                   // copy constructor
  ~SparseMtx(){ delete[] sra; delete[] fnz; delete[] clm; }
	
  SparseMtx& operator=(const SparseMtx&);        // overload =
  Vcr<T> operator*(const Vcr<T>&) const;         // matrix vector multiply
  T& operator[](int i) const { return sra[i]; }  // subscripting
  int& getfnz(int i) const { return fnz[i]; }    // 1st nonzero entry ofeach row
  int& getclm(int i) const { return clm[i]; }    // column index
};

template<class T> 
class BandMtx: public AbsMtx<T> {                // banded matrix
  using AbsMtx<T>::nrows;
private:
  int bwleft;                                    // left bandwidth 
  int bwrit;                                     // right bandwidth 
  T** bdmx;                                      // entries within the band 
  Vcr<T> preconding(const Vcr<T> &, int = 0) const; 
public: 
  BandMtx(int n, int p, int r, T** t);           // n: # of rows = # of columns
      // p: left bandwidth, r: right bandwidth, t: entries within the band 

  BandMtx(int n, int p, int r, T t = 0);         // initialize all entries to t
  BandMtx(int n, int p, int r, const FullMtx<T>& m);    // m: full matrix format
  BandMtx(const BandMtx &);                      // copy constructor
  ~BandMtx(){ 
    for (int i = 0; i < nrows; i++) delete[] (bdmx[i] -= bwleft);
    delete[] bdmx; 
  }

  BandMtx& operator=(const BandMtx&);            // overload =
  T* operator[](int i) const { return bdmx[i]; } // i-th row of bdmx
  Vcr<T> operator*(const Vcr<T>&) const;         // matrix vector multiply
  void GaussElim(Vcr<T>& b) const;               // Gauss elimination
  void GaussElimPP(Vcr<T>& b) const;             // Gauss elim, column pivoting
  void GaussElimPP2(Vcr<T>& b) const;            // Gauss elim, column pivoting
  //void LUdecompn();                            // LU decomposition
  //Vcr<int> LUdecompnCP();                      // LU decompn, column pivoting
};

template<class T> class Vcr {       
  int lenth;                                     // number of entries 
  T* vr;                                         // entries of the vector
public: 
  Vcr(int n, T* t);                              // constructor from t
  Vcr(int n, T t = 0);                          // constructor, all entries =t
  Vcr(const Vcr&);                               // copy constructor
  ~Vcr(){ delete[] vr; }                         // destructor

  int size() const { return lenth; }             // return size of vector
  void reset(T = 0);                             // reset all enties to a value 
  T& operator[](int i) const { return vr[i]; }   // v[i] = 10;
  Vcr& operator=(const Vcr&);                    // overload =, v = v2
  Vcr& operator+=(const Vcr&);                   // v += v2
  Vcr& operator-=(const Vcr&);                   // v -= v2
  T maxnorm() const;                             // maximum norm
  T twonorm() const;                             // L-2 norm

  template <class S> friend 
  Vcr<S> operator+(const Vcr<S>&);               // unary operator, v = + v2

  template <class S> friend 
  Vcr<S> operator-(const Vcr<S>&);               // unary operator, v = - v2

  template <class S> friend 
  Vcr<S> operator+(const Vcr<S>&, const Vcr<S>&);  // v= v1 + v2

  template <class S> friend 
  Vcr<S> operator-(const Vcr<S>&, const Vcr<S>&);  // v= v1 - v2

  template <class S> friend 
  S dot(const Vcr<S>&, const Vcr<S>&);           // dot product

  template <class S> friend 
  Vcr<S> operator*(S, const Vcr<S>&);            // vec-scalar multiply 

  template <class S> friend 
  Vcr<S> operator*(const Vcr<S>&, S);            // vec-scalar multiply 

  template <class S> friend 
  Vcr<S> operator*(const Vcr<S>&, const Vcr<S>&);  // component-wise multiply

  template <class S> friend 
  Vcr<S> operator/(const Vcr<S>&, S);            // vec-scalar division 

  template <class S> friend 
  ostream& operator<<(ostream&, const Vcr<S>&);    
};

template<class T> T dot(T* a, T* b, int n);

// a vector class
template<>
class Vcr< complex<double> > {                   // a complete specialization
  int lenth;                                     // number of entries in vector
  complex<double>* vr;                           // entries of the vector
public: 
  Vcr(int, complex<double> *cp);                 // construct a vector from cp
  Vcr(int, complex<double> c = 0);      // constructor, all entries = c
  Vcr(const Vcr&);                               // copy constructor
  ~Vcr(){ delete[] vr;}                          // destructor

  int size() const { return lenth; }             // return size of vector
  void reset(complex<double> t = 0);             // reset all enties to t
  complex<double> & operator[](int i) const { return vr[i]; } 
  Vcr& operator=(const Vcr&);    
  Vcr& operator+=(const Vcr&);                   // v += v2
  Vcr& operator-=(const Vcr&);                   // v -= v2
  double maxnorm() const;                        // maximum norm
  double twonorm() const;                        // L-2 norm
  friend Vcr operator+(const Vcr&);              // unary +, v = + v2
  friend Vcr operator-(const Vcr&);              // unary -, v = - v2
  friend Vcr operator+(const Vcr&, const Vcr&);  // v= v1+v2
  friend Vcr operator-(const Vcr&, const Vcr&);  // v= v1-v2
  friend complex<double> dot(const Vcr&, const Vcr&);      // dot product
  friend Vcr operator*(complex<double>, const Vcr&);       // vec-scalar x 
  friend Vcr operator*(const Vcr&, complex<double>);       // vec-scalar x
  friend Vcr operator*(const Vcr&, const Vcr&);            // component-wise x  
  friend Vcr operator/(const Vcr&, complex<double>);       // vec-scalar / 
  friend ostream& operator<<(ostream&, const Vcr&);    
};

complex<double> dot(complex<double>* a, complex<double>* b, int n);

//***** some usefull small functions and parameters *******

//*** report error and exit with return value 1.
template<class T> void error(const T& t, ostream& s = cout) {
//template<class T> void error(const T& t) {
  cout << t << ". program exited." << "\n";
  exit(1);
}

const double SMALL = 1.0e-15;
//const double SMALL = numeric_limits<double>::epsilon(); 


// *** definitions for members of class FullMtx

template<class T> 
FullMtx<T>::FullMtx(int n, int m, T** dbp) {
  nrows = n;
  ncols = m;
  mx = new T* [nrows]; 
  for (int i =  0; i< nrows; i++) {
    mx[i] = new T [ncols];          
    for (int j = 0; j < ncols; j++) mx[i][j] = dbp[i][j];
  }
}

template<class T> 
FullMtx<T>::FullMtx(int n, int m, T a) {
  nrows = n;
  ncols = m;
  mx = new T* [nrows]; 
  for (int i =  0; i< nrows; i++) {
    mx[i] = new T [ncols];
    for (int j = 0; j < ncols; j++) mx[i][j] = a;
  }
}

template<class T> 
FullMtx<T>::FullMtx(const FullMtx& mat) { 
  nrows = mat.nrows;
  ncols = mat.ncols;
  mx = new T* [nrows]; 
  for (int i =  0; i< nrows; i++) {
    mx[i] = new T [ncols];
    for (int j = 0; j < ncols; j++) mx[i][j] = mat[i][j]; 
  }
}

template<class T> 
FullMtx<T> & FullMtx<T>::operator=(const FullMtx & mat) {
  if (this != &mat) {
    if (nrows !=mat.nrows || ncols !=mat.ncols) 
      error("bad matrix sizes in FullMtx::operator=()");
    for (int i = 0; i < nrows; i++) 
      for (int j = 0; j < ncols; j++) mx[i][j]  = mat.mx[i][j];
  }
  return *this;
}

template<class T>                     // usefull for testing small matrices 
ostream& operator<<(ostream& s, const FullMtx<T>& mat) {
  for (int i =0; i< mat.nrows; i++) {
    s << "\n" << i << "-th row:    \n";
    for (int j =0; j< mat.ncols; j++) {
      s << mat.mx[i][j] << "  ";
      if (j%10 ==9) s << "\n";
    }
    s << "\n";
  }
  return s;
}

template<class T> 
FullMtx<T> & FullMtx<T>::operator+=(const FullMtx &  mat) {
  if (nrows !=mat.nrows || ncols !=mat.ncols) 
    error("bad matrix sizes in FullMtrx::operator+=()");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++) mx[i][j] += mat[i][j];
  return *this;
}

template<class T> 
FullMtx<T> & FullMtx<T>::operator-=(const FullMtx &  mat) {
  if (nrows !=mat.nrows || ncols !=mat.ncols) 
    error("bad matrix sizes in FullMtrx::operator-=()");
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++) mx[i][j] -= mat[i][j];
  return *this;
}

template<class T> 
FullMtx<T> & FullMtx<T>::operator+() {                // usage: mat1 = + mat2;
  return *this;
}

template<class T> 
FullMtx<T> operator-(const FullMtx<T>&  mat) {        // usage: mat1 = - mat2;
  FullMtx<T> minus = mat;  
  for (int i = 0; i < mat.nrows; i++) {
    for (int j = 0; j < mat.ncols; j++) minus[i][j] = - mat[i][j];
  }
  return minus;
}

template<class T> 
FullMtx<T> FullMtx<T>::operator+(const FullMtx& mat) { //m =m1+m2
 FullMtx sum = *this;
 sum += mat;
 return sum;
}

template<class T> 
Vcr<T> FullMtx<T>::operator*(const Vcr<T> & vec) const {
  if (ncols != vec.size()) 
    error("matrix and vector sizes do not match in FullMtx::operator*()");
  Vcr<T> tm(nrows);
  for (int i = 0; i < nrows; i++) 
    for (int j = 0; j < ncols; j++) tm[i] += mx[i][j]*vec[j]; 
  return tm;
}

template<class T> 
FullMtx<T> operator-(const FullMtx<T> & mat1, const FullMtx<T>& mat2) {
 if(mat1.nrows !=mat2.nrows || mat1.ncols !=mat2.ncols) 
   error("bad matrix sizes in FullMtx subtraction");
 FullMtx<T> sum = mat1;
 sum -= mat2;
 return sum;
}

template<class T>                 // solve Pz = r, return z. (P: preconditioner)
Vcr<T> FullMtx<T>::preconding(const Vcr<T>& r, int precn) const {
  if (precn == 0) {               // no preconditioner is used
    return r;
  } else if (precn == 1) {        // diagonal preconditioning
    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) z[i] = r[i]/mx[i][i];
    return z;
  } else if (precn == 2) {        // symmetric SOR preconditioning 
    const T omega = 1;            // SSOR parameter for preconditioning
    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) {
      T sum = 0;
      for (int j = 0; j < i; j++)  sum += omega*mx[i][j]*z[j];
      z[i] = (r[i] - sum)/mx[i][i];
    } 
    for (int i = nrows -1; i >= 0; i--) {
      T sum = 0;
      for (int j = i +1; j < nrows; j++) sum += omega*mx[i][j]*z[j];
      z[i] -= sum/mx[i][i];
    }
    return z;
  } else {
    error("specified preconditioner in FullMtx::preconding() not implemented");
  } 
} // end FullMtx::preconding()


/*
template<class T>          // LU decomposition with partial (column) pivoting
Vcr<int> FullMtx<T>::LUdecompnCP() {
  Vcr<int> pvt(nrows);                          // store pivoting info
  for (int k = 0; k < nrows; k++) pvt[k] = k;

  for (int k = 0; k < nrows - 1; k++) {
    // find the pivot in column k below main diagonal
    int pc  = k; 
    double aet = abs(mx[k][k]);
    for (int i = k + 1; i < nrows; i++) {
      if (abs(mx[i][k]) > aet) {
        aet = abs(mx[i][k]); 
        pc = i;
      }
    }
    if (aet == 0) error("pivot is zero in FullMtx::LUdecompn()"); 
    swap(pvt[k], pvt[pc]);

    // interchange pivot row and k-th row
    if (pc != k)
      for (int j = k; j < ncols; j++) swap(mx[pc][j], mx[k][j]);

    // now eliminate the column entries below tmx[k][k]
    for (int i = k + 1; i < nrows; i++) {
      if (mx[i][k] != 0) {
        T mult = mx[i][k]/mx[k][k];
        mx[i][k] = mult;
        for (int j = k + 1; j < ncols; j++) mx[i][j] -= mult*mx[k][j];
      }
    }
  }
  return pvt;
} // end LU decompnCP()   
*/

template<class T>     // Gauss Elim without pivoting
void FullMtx<T>::GaussElim(Vcr<T>& bb) const{
  if (nrows != bb.size() || nrows != ncols) 
    error("matrix or vector sizes do not match");
  FullMtx<T> tmpx = *this;
  
  // LU decomposition without pivoting 
  int nrowsmone = nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {
    if (tmpx[k][k] == 0.0) 
      error("pivot is zero in FullMtx::GaussElim()");
    for (int i = k + 1; i < nrows; i++) {
      if (tmpx[i][k] != 0.0) {   // tmpx[i][k] can be complex
        T mult = tmpx[i][k]/tmpx[k][k];
        tmpx[i][k] = mult;
        for (int j = k + 1; j < ncols; j++) 
          tmpx[i][j] -= mult*tmpx[k][j];
      }
    }
  }

  // forwad substitution for L y = b. y still stored in bb
  for (int i = 1; i < nrows; i++) 
    for (int j = 0; j < i; j++) bb[i] -= tmpx[i][j]*bb[j];

  // back substitution for U x = y. x still stored in bb
  for (int i = nrowsmone; i >= 0; i--) {
    for (int j=i+1; j<nrows; j++) bb[i] -= tmpx[i][j]*bb[j];
    bb[i] /= tmpx[i][i];
  }
} // end GaussElim()

// Gauss Elim with partial pivoting
// this version does not do row interchanges
template<class T>     
void FullMtx<T>::GaussElimPP(Vcr<T>& bb) const {
  if (nrows != bb.size() || nrows != ncols) 
    error("matrix or vector sizes do not match");
  FullMtx<T> tmpx = *this;

  Vcr<int> pvt(nrows);       // pivoting vector 
  for (int k = 0; k < nrows; k++) pvt[k] = k;

  int nrowsmone = nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {  // main loop

    // find the pivot in column k in rows pvt[k], 
    // pvt[k+1], ..., pvt[n-1]
    int pc  = k; 
    double aet = abs(tmpx[pvt[k]][k]);
    for (int i = k + 1; i < nrows; i++) {
      if (abs(tmpx[pvt[i]][k]) > aet) {
        aet = abs(tmpx[pvt[i]][k]); 
        pc = i;
      }
    }
    if (!aet) 
      error("pivot is zero in FullMtx::GaussElimPtP()"); 
    if (pc != k) swap(pvt[k], pvt[pc]);
    int pvtk = pvt[k];                  // pivot row
    T pivot = tmpx[pvtk][k];            // pivot

    // now eliminate column entries logically 
    // below tmpx[pvt[k]][k]
    for (int i = k + 1; i < nrows; i++) {
      int pvti = pvt[i];
      if (tmpx[pvti][k] != 0) {
        T mult = tmpx[pvti][k]/pivot;
        tmpx[pvti][k] = mult;
        for (int j = k + 1; j < ncols; j++) 
          tmpx[pvti][j] -= mult*tmpx[pvtk][j];
      }
    }
  }

  // forwad substitution for L y = Pb.
  for (int i = 1; i < nrows; i++)  
    for (int j = 0; j < i; j++) 
      bb[pvt[i]] -= tmpx[pvt[i]][j]*bb[pvt[j]];

  // back substitution for Ux = y
  Vcr<T> xx(nrows);  // xx stores solution in correct order
  for (int i = nrowsmone; i >= 0; i--) {
    for (int j = i+1; j < ncols; j++) 
      bb[pvt[i]] -= tmpx[pvt[i]][j]*xx[j];
    xx[i] = bb[pvt[i]] / tmpx[pvt[i]][i];
  }

  bb = xx;             // put solution in bb
} // end GaussElimPP()


// Gauss Elim with partial (column) pivoting
// this version does row interchanges, which normally
// should be less efficient
template<class T>   
void FullMtx<T>::GaussElimPP2(Vcr<T>& bb) const {

  // this function interchanges row k and pivot row

  if (nrows != bb.size() || nrows != ncols) 
    error("matrix or vector sizes do not match in FullMtx::GaussElimPtP()");
  FullMtx<T> tmpx = *this;

  Vcr<int> pvt(nrows);                          // pivoting vector 
  for (int k = 0; k < nrows; k++) pvt[k] = k;

  const int nrowsmone = nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {         // main loop

    // find the pivot in column k below tmpx[k][k]
    int pc  = k; 
    double aet = abs(tmpx[k][k]);
    for (int i = k + 1; i < nrows; i++) {
      if (abs(tmpx[i][k]) > aet) {
        aet = abs(tmpx[i][k]); 
        pc = i;
      }
    }
    if (!aet) error("pivot is zero in FullMtx::GaussElimCP()"); 
    if (pc != k) {
      swap(pvt[k], pvt[pc]);
      for (int j = 0; j < ncols; j++) swap(tmpx[pc][j], tmpx[k][j]);
    }

    // now eliminate the column entries below tmpx[k][k]
    for (int i = k + 1; i < nrows; i++) {
      if (tmpx[i][k] != 0.0) {
        T mult = tmpx[i][k]/tmpx[k][k];
        tmpx[i][k] = mult;
        for (int j = k + 1; j < ncols; j++) 
          tmpx[i][j] -= mult*tmpx[k][j];
      }
    }
  }

  Vcr<T> rhv(nrows);
  for (int k = 0; k < nrows; k++) rhv[k] = bb[pvt[k]];
  bb = rhv;

  // forwad substitution for L y = Pb.
  for (int i = 1; i < nrows; i++)  
    for (int j = 0; j < i; j++) bb[i] -= tmpx[i][j]*bb[j];

  // back substitution  for Ux = y
  for (int i = nrowsmone; i >= 0; i--) {
    for (int j = i+1; j < ncols; j++) bb[i] -= tmpx[i][j]*bb[j];
    bb[i] /= tmpx[i][i];
  }

} // end GaussElimPP2()

template<class T>    // Gauss Elim with complete pivoting
void FullMtx<T>::GaussElimCP(Vcr<T>& bb) const {
  if (nrows != bb.size() || nrows != ncols) 
    error("matrix vector sizes no match in GaussElimCP()");
  FullMtx<T> tmpx = *this;

  Vcr<int> px(nrows);             // row pivoting vector 
  Vcr<int> qy(nrows);             // column pivoting vector 
  for (int k = 0; k < nrows; k++) px[k] = qy[k] = k;

  const int nrowsmone = nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {  // main loop

    // find pivot entry in columns qy[k], qy[k+1], ..., 
    // qy[n-1] and in rows px[k], px[k+1], ..., px[n-1]
    int pct = k, qdt = k; 
    double aet = 0;
    for (int i = k; i < nrows; i++) {
      for (int j = k; j < nrows; j++) {
        double tmp = abs(tmpx[px[i]][qy[j]]); 
        if (tmp > aet) { aet = tmp; pct = i; qdt = j; }
      }
    }
    if (!aet) error("pivot is zero in GaussElimCP()"); 
    swap(px[k], px[pct]);    // swap px[k] and px[pct]
    swap(qy[k], qy[qdt]);

    // eliminate column entries logically below and right
    // to the entry mx[px[k]][qy[k]]
    for (int i = k + 1; i < nrows; i++) {
      if (tmpx[px[i]][qy[k]] != 0.0) {
        T mult = tmpx[px[i]][qy[k]]/tmpx[px[k]][qy[k]]; 
        tmpx[px[i]][qy[k]] = mult;
        for (int j = k + 1; j < nrows; j++) 
          tmpx[px[i]][qy[j]] -= mult*tmpx[px[k]][qy[j]];
      }
    }
  }

  // forwad substitution for L y = Pb. Store y in b
  for (int i = 1; i < nrows; i++)  
    for (int j = 0; j < i; j++) 
      bb[px[i]] -= tmpx[px[i]][qy[j]]*bb[px[j]];

  // back substitution for Uz = y and x = Q z
  Vcr<T> xx(nrows);    // xx stores solution      
  for (int i = nrowsmone; i >= 0; i--) {
    for (int j = i+1; j < nrows; j++) 
      bb[px[i]] -= tmpx[px[i]][qy[j]]*xx[qy[j]];
    xx[qy[i]] = bb[px[i]] / tmpx[px[i]][qy[i]];
  }

  bb = xx;
} // end GaussElimCP()


// *** definitions for members in class Vcr.

template<class T> 
Vcr<T>::Vcr(int n, T* abd) {
  vr = new T [lenth = n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd +i);
}

template<class T> 
Vcr<T>::Vcr(int n, T a) {
  vr = new T [lenth = n]; 
  for (int i = 0; i < lenth; i++)  vr[i] = a;
}

template<class T> 
Vcr<T>::Vcr(const Vcr & vec) {
  vr = new T [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec.vr[i]; 
}

template<class T> 
void Vcr<T>::reset(T a) {
 for (int i = 0; i < lenth; i++)  vr[i] = a; 
}

template<class T> 
Vcr<T>& Vcr<T>::operator=(const Vcr<T>& vec) {
  if (this != &vec) {
    if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator=()");
    for (int i = 0; i < lenth; i++) vr[i] = vec.vr[i];
  }
  return *this;
}

template<class T> 
Vcr<T> & Vcr<T>::operator+=(const Vcr<T>& vec) {
  if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator+=()");
  for (int i = 0; i < lenth; i++) vr[i] += vec.vr[i];
  return *this;
}

template<class T> 
Vcr<T> & Vcr<T>::operator-=(const Vcr<T>& vec) {
  if (lenth != vec.lenth) error("bad vector sizes in Vcr::operator-=()");
  for (int i = 0; i < lenth; i++) vr[i] -= vec.vr[i];
  return *this;
}

template<class T> 
Vcr<T> operator+(const Vcr<T>& vec) {             // usage: vec1 = + vec2;
  return vec;
}
 
template<class T> 
Vcr<T> operator-(const Vcr<T>& vec) {             // usage: vec1 = - vec2;
  Vcr<T> minus = vec;
  for (int i = 0; i < vec.lenth; i++) minus[i] = - vec[i];
  return minus;
}

template<class T> 
Vcr<T> operator+(const Vcr<T>& v1, const Vcr<T>& v2) {         // v=v1+v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vecor addition");
  Vcr<T> sum = v1; 
  sum += v2;
  return sum;
}

template<class T> 
Vcr<T> operator-(const Vcr<T>& v1, const Vcr<T> & v2) {         // v=v1-v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vector subtraction");
  Vcr<T> sum = v1;           // It would cause problem without copy constructor
  sum -= v2;
  return sum;
}

template<class T> 
ostream &  operator<<(ostream & s, const Vcr<T> & vec ) {
  for (int i =0; i < vec.lenth; i++ ) {
    s << vec[i] << "  ";
    if (i%10 == 9) s << "\n";
  }
  return s;
}

template<class T> T Vcr<T>::twonorm() const {
  T nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    T vi = abs(vr[i]);
    if (nm < 100) nm = sqrt(nm*nm + vi*vi);
    else {                  // to avoid overflow for fn "sqrt" when nm is large
      T tm = vi/nm;
      nm *= sqrt(1.0 + tm*tm);
    }
  } 
  return nm;
}

template<class T> T Vcr<T>::maxnorm() const {
  T nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    T vi = abs(vr[i]);
    if (nm < vi) nm = vi;
  }
  return nm;
}

template<class T> 
Vcr<T> operator*(T scalar, const Vcr<T> & vec) {
  Vcr<T> tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = scalar*vec[i]; 
  return tm;
}

template<class T> 
Vcr<T> operator*(const Vcr<T>& vec, T scalar) {
  Vcr<T> tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = scalar*vec[i]; 
  return tm;
}

template<class T> 
Vcr<T> operator*(const Vcr<T> & vec1, const Vcr<T> & vec2) {
  if (vec1.lenth != vec2.lenth ) error("bad vector sizes in vector multiply");
  Vcr<T> tm(vec1.lenth);
  for (int i = 0; i < vec1.lenth; i++) tm[i] = vec1[i]*vec2[i]; 
  return tm;
}

template<class T> 
Vcr<T> operator/(const Vcr<T> & vec, T scalar) {
  if (scalar == 0) error("divisor is zero in vector-scalar division");
  Vcr<T> tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = vec[i]/scalar; 
  return tm; 
}

template<class T> 
T dot(const Vcr<T> & v1, const Vcr<T> & v2) {
  if (v1.lenth != v2.lenth ) error("bad vector sizes in dot product");
  T tm = v1[0]*v2[0];
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*v2[i];
  return tm;
}

template<class T> T dot(T* a, T* b, int n) {
  T init = 0;
  for (int i = 0; i < n; i++) init += *a++ * *b++;
  return init;
}

// *** definitions of iterative solvers for AbsMtx

template<class T>
int AbsMtx<T>::CG(Vcr<T>& x, const Vcr<T>& b, double& eps,  
	                       int& iter, int prec) {
// Conjugate gradient method. 
// x: on entry, initial guess; on retrun: approximate solution 
// b: right hand side vector as in A x = b;
// eps:  on entry, tolerance; on retrun: absolute residual in Euclid norm 
// iter: on entry, max number of iterations allowed; 
//       on return, actual number of iterations used 
// prec= 0 if no preconditioning, 1 if diag prec, 2 if SSOR prec

  if (nrows != b.size()) error("matrix and vector sizes do not match");
  const int maxiter = iter;
  Vcr<T> r = b - (*this)*x;                   // initial residual 
  Vcr<T> z = preconding(r,prec);              // solve the precond system
  Vcr<T> p = z;                               // p: search direction
  T zr = dot(z,r);                            // inner prod of z and r
  const double stp = eps*b.twonorm();         // stopping criterion

  if (r.maxnorm() == 0.0) {                   // if intial guess is true soln, 
    eps = 0.0;                                // return. Otherwise division by 
    iter = 0;                                 // zero would occur.
    return 0; 
  }

  for (iter = 0; iter < maxiter; iter++) {     // main loop of CG method
    Vcr<T> mp = (*this)*p;                     // one matrix-vector multiply
    T pap = dot(mp,p);                         // one of two inner products
    T alpha = zr/pap;                          // pap is 0 only when r is 0
    x += alpha*p;                              // update the iterative soln
    r -= alpha*mp;                             // update residual 
    if (r.twonorm() <= stp) break;             // stop if convergence achieved
    z = preconding(r,prec);                    // preconditioning
    T zrold = zr;
    zr = dot(z,r);                             // another of two inner products
    T beta= zr/zrold;                          // zrold = 0 only when r is 0
    p = z + beta*p;                            // update search direction
  } 

  eps = r.twonorm();
  if (iter == maxiter) return 1;
  else return 0;
} // end CG()

template<class T>
int AbsMtx<T>::GMRES(Vcr<T>& x, const Vcr<T>& b, double& eps,
                     int& iter, int pcn, int m) {  
    // Restarted GMRES for solving Ax = b, called GMRES(m) 
    // Return 0 if sucessful and 1 if breakdown
    // It outputs the approximate solution, residual, and 
    // number of iterations taken. 
    //
    // A: any nonsingular matrix, may not be symmetric
    // x: on entry:  initial guess
    //    on return: approximate solution
    // b: right side vector
    // eps: on input, stopping criterion,
    //      on return, absolute two-norm residual
    // iter: on input, max number of iterations allowed 
    //       on return, actual number of iterations taken.
    // pcn: = 0 if no preconditioner, = 1 if diag precond, 
    //      = 2 if SSOR precond
    // m: number of iterations for restarting GMRES

  const int maxiter = iter;
  const double stp = (preconding(b, pcn)).twonorm() * eps;
  Vcr<T> r = preconding(b - (*this) * x, pcn);
  T beta = r.twonorm();             // T may be complex<C>
  bool conv = false;
  if (m > nrows) 
    error("In GMRES(m), m is bigger than number of rows");
  if (abs(beta) <= stp) {    // return if initial guess 
    eps = abs(beta);         // is true solution
    iter = 0;
    return 0;
  }

  // orthonormal basis for Krylov space,
  // v[i] is a pointer to ith basis vector
  Vcr<T>** v = new Vcr<T>* [m+1];    
  for (int i = 0; i <= m; i++) 
    v[i] = new Vcr<T>(nrows);  // ith orthonormal basis

  // Hessenburg matrix h, (m+1) by m;
  // h[i] stores ith column of h that has i+2 entries.
  // Only non-zero part of h is stored
  T** h = new T* [m];     
  for (int i = 0; i < m; i++) h[i] = new T [i+2];              

  iter = 1;
  while (iter  <= maxiter) {  // iterations for gmres(m)
    *v[0] = r / beta;  
    Vcr<T> g(m+1);    // vector in least squares problem
    g[0] = beta;

    Vcr<T> cs(m), sn(m);      // Givens rotations
    int k;  
    for (k = 0; k < m && iter <= maxiter; k++, iter++) {

      // orthogonalization
      Vcr<T> w = preconding((*this) * (*v[k]), pcn);
      T nmw = w.twonorm();
      for (int i = 0; i <= k; i++) {     
        h[k][i] = dot(w, *v[i]);
        w -= h[k][i] * (*v[i]);
      }
      h[k][k+1] = w.twonorm();
      // if h[k][k+1] is small, do reorthogonalization
      if (nmw + 1.0e-4*h[k][k+1] == nmw) {
        for (int i = 0; i <= k; i++) { 
          T hri = dot(w, *v[i]);   // re-orthogonalization
          h[k][i] += hri;
          w -= hri * (*v[i]);
        }
        h[k][k+1] = w.twonorm();
      }
      if (h[k][k+1] == 0.0) 
        error("unexpected zero-divisor in GMRES()");
      *v[k+1] = w / h[k][k+1]; 

      // apply Givens G_0, G_1, ...,G_{k-1} to column k of h
      for (int i = 0; i < k; i++) { 
        // T tmp = conj(cs[i])*h[k][i] + conj(sn[i])*h[k][i+1];
        T tv[2] = { cs[i], sn[i] };
        T tmp = dot(&h[k][i], tv, 2);
        h[k][i+1] = - sn[i]*h[k][i] + cs[i]*h[k][i+1];
        h[k][i] = tmp;
      }

      // generate Givens rotation G_k from kth column of h
      if (h[k][k+1] == 0.0) {
        cs[k] = 1;
        sn[k] = 0;
      } else {
        T tpm = sqrt(dot(&h[k][k], &h[k][k], 2));
        cs[k] = h[k][k]/tpm;
        sn[k] = h[k][k+1]/tpm;
      }

      // apply Givens rotation G_k to kth column of h and to g
      T tv[2] = { cs[k], sn[k] };
      h[k][k] = dot(&h[k][k], tv, 2);
      T tmp = dot(&g[k], tv, 2);
      g[k+1] = - sn[k]*g[k] + cs[k]*g[k+1];
      g[k] = tmp;

      if (abs(g[k+1]) <= stp) { // stop if residual small
        k++;
        break;
      }
    }

    // back solve the upper triangular system
    for (int i = k-1; i >= 0; i--){
      for (int j = i+1; j < k; j++) g[i] -= h[j][i]*g[j];
      g[i] /= h[i][i];
    }

    // update the solution x = x0 + sum v[i]*y[i]
    for (int i = 0; i < k; i++) x += (*v[i])*g[i];

    // calculate residual and check for convergence
    r = preconding(b - (*this) * x, pcn);
    beta = r.twonorm();
    if (abs(beta) <= stp ) {    // stop if residual small
      conv = true;
      break;
    }
  }

  eps =  (b - (*this) * x).twonorm();  // get final residual

  // deallocate space for v and h
  for (int i = 0; i <= m; i++ ) delete v[i];
  delete[] v;
  for (int i = 0; i < m; i++) delete[] h[i];
  delete[] h;

  if (conv) return 0;
  else return 1;
}   // end of gmres(m)


// *** definitions for members of class BandMtx

template<class T>
BandMtx<T>::BandMtx(int n, int p, int r, T** t) {
  nrows = n;                                 // number of rows
  bwleft = p;                                // left bandwidth
  bwrit = r;                                 // right bandwidth
  bdmx = new T* [nrows]; 
  for (int i = 0; i < nrows; i++) {
   bdmx[i] = new T [bwleft + bwrit + 1];
   bdmx[i] += bwleft;                              
  }
  for (int i = 0; i < nrows; i++) 
    for (int j = - bwleft; j <= bwrit; j++) bdmx[i][j] = t[i][j];
}

template<class T>
BandMtx<T>::BandMtx(int n, int p, int r, T t) {
  nrows = n;
  bwleft = p;
  bwrit = r;
  bdmx = new T* [nrows]; 
  for (int i = 0; i < nrows; i++) {
   bdmx[i] = new T [bwleft + bwrit + 1];
   bdmx[i] += bwleft;                              
  }
  for (int i = 0; i < nrows; i++) 
    for (int j = - bwleft; j <= bwrit; j++) bdmx[i][j] = t;
}

template<class T>
BandMtx<T>::BandMtx(const BandMtx & bd) {
  nrows = bd.nrows;
  bwleft = bd.bwleft;
  bwrit = bd.bwrit;
  bdmx = new T* [nrows]; 
  for (int i = 0; i < nrows; i++) {
   bdmx[i] = new T [bwleft + bwrit + 1];
   bdmx[i] += bwleft;                              
  }
  for (int i = 0; i < nrows; i++) 
    for (int j = - bwleft; j <= bwrit; j++) bdmx[i][j] = bd[i][j];
}


template<class T>
BandMtx<T>::BandMtx(int n, int p, int r, const FullMtx<T>& fm) {
  nrows = n;
  bwleft = p;
  bwrit = r;
  bdmx = new T* [nrows]; 
  for (int i = 0; i < nrows; i++) {
   bdmx[i] = new T [bwleft + bwrit + 1];
   bdmx[i] += bwleft;                              
  }
  for (int i = 0; i< nrows; i++) {
    for (int j = - bwleft; j <= bwrit; j++) 
    bdmx[i][j] = 0;
  }
  for (int i = 0; i< nrows; i++) {                   // fm may be non-symmetric
    int ip = max(i-bwleft, 0); 
    int ir = min(i+bwrit, nrows -1); 
    for (int j = ip; j <= ir; j++) bdmx[i][j-i] = fm[i][j];
  }
}

template<class T>
BandMtx<T>& BandMtx<T>::operator=(const BandMtx & bd) {
  if (nrows != bd.nrows || bwleft != bd.bwleft || bwrit != bd.bwrit) 
    error("bad matrix size in BandMtx assignment");
  for (int j = 0; j < nrows; j++) 
    for (int k = -bwleft; k <= bwrit; k++) bdmx[j][k] = bd[j][k];
  return *this;
}

template<class T>                                     // matrix vector multiply 
Vcr<T> BandMtx<T>::operator*(const Vcr<T> & vec) const {    
  if (nrows != vec.size()) error("matrix and vector sizes do not match");
  Vcr<T> tm(nrows);
  for (int i = 0; i < nrows; i++) {
    int ip = max(i-bwleft, 0); 
    int ir = min(i+bwrit, nrows - 1);
    for (int j = ip; j <= ir; j++) tm[i] += bdmx[i][j-i]*vec[j]; 
  }
  return tm;
}

template<class T>                      // solve Pz = r, return z
Vcr<T> BandMtx<T>::preconding(const Vcr<T> & r, int precn) const {
  if (precn == 0) {                    // no preconditioning
    return r;
  } else if (precn == 1) {             // diagonal preconditioning
    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) z[i] = r[i]/bdmx[i][0];
    return z;
  } else if (precn == 2) {             // symmetric SOR preconditioning
    const T omega = 1;                 // SSOR parameter for preconditioning
    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) {
      T sum = 0;
      int ip = max(i-bwleft, 0);
      for (int j = ip; j < i; j++)  sum += omega*bdmx[i][j-i]*z[j];
      z[i] = (r[i] - sum)/bdmx[i][0];
    }
    for (int i = nrows -1; i >= 0; i--) {
      T sum = 0;
      int ir = min(i+bwrit, nrows -1); 
      for (int j = i+1; j <= ir; j++) sum += omega*bdmx[i][j-i]*z[j];
      z[i] -= sum/bdmx[i][0];
    }
    return z;
  } else {
    error("specified preconditioner in BandMtx::preconding() not implemented");
  }
}

// BandGauss
template<class T>
void BandMtx<T>::GaussElim(Vcr<T>& bb) const {
                           // Gauss elim without pivoting
  if (nrows != bb.size()) 
    error("matrix and vector sizes do not match");
  BandMtx<T> tmx = *this;

  // banded LU decomposition without pivoting
  const int nrowsmone = nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {
    if (tmx[k][0] == 0.0) 
      error("pivot is zero in BandMtx::GaussElim()");
    int kbf = min(nrowsmone - k, bwleft);
    int kbr = min(nrowsmone - k, bwrit);
    for (int i = 1; i <= kbf; i++) {
      int kpi = k+i;
      if (tmx[kpi][-i] != 0.0) {   
        T dmul = tmx[kpi][-i]/tmx[k][0];
        tmx[kpi][-i] = dmul;  // tmx[k+i][-i] = a[k+i][k]
        for (int j = 1; j <= kbr; j++) 
          tmx[kpi][j-i] -= dmul*tmx[k][j];   
      }                       // a[k+i][k+j] = tmx[k+i][j-i]
    }
  }

  // Forward substitution
  for (int k = 1; k < nrows; k++) {
    int kf = min(k, bwleft);
    for (int j = 1; j <= kf; j++) 
      bb[k] -= tmx[k][-j]*bb[k-j];
  }                           // tmx[k][-j] = a[k][k-j]

  // Backward substitution
  for (int k = nrowsmone; k >= 0; k--) {
    int kbr = min(nrowsmone - k, bwrit);
    for ( int j = 1; j <= kbr; j++) 
      bb[k] -= tmx[k][j]*bb[k+j];
    bb[k] /= tmx[k][0];
  }
} // end GaussElim()

// BandGauss elimination with partial  pivoting
template<class T>
void BandMtx<T>::GaussElimPP(Vcr<T>& bb) const {
  if (nrows != bb.size()) 
    error("matrix-vector sizes not match");

  BandMtx<T> tx(nrows, bwleft,
                       min(nrows-1, bwleft+bwrit));
  for (int i = 0; i < nrows; i++) 
    for (int j = - bwleft; j <= bwrit; j++) 
      tx[i][j] = bdmx[i][j];

  Vcr<int> pvt(nrows);   // store pivoting info

  // LU decomposition with column partial pivoting
  const int nrowsmone = tx.nrows - 1;
  for (int k = 0; k < nrowsmone; k++) {
    int kbrow = min(nrowsmone - k, tx.bwleft); 
    int kbcol = min(nrowsmone - k, tx.bwrit); 

    // find the pivot in the k-th column
    int pc = k;
    double aet = abs(tx[k][0]);
    for (int i = 1; i <= kbrow; i++) {
      if (abs(tx[k+i][-i]) > aet) {
        aet = abs(tx[k+i][-i]);
        pc = k + i;
      }
    }
    if (!aet) error("pivot is zero in GaussElimPP");
    pvt[k] = pc;

    // interchange pivot row and k-th row in U, not in L
    if (pc != k) {
      for (int j = 0; j <= kbcol; j++) 
        swap(tx[pc][k+j-pc], tx[k][j]);
    }

    // now eliminate column entries 
    for (int i = 1; i <= kbrow; i++) { 
      int kpi = k + i;
      if (tx[kpi][-i] != 0.0) {
        T dmul = tx[kpi][-i]/tx[k][0];
        tx[kpi][-i] = dmul;
        for (int j = 1; j <= kbcol; j++) 
          tx[kpi][j-i] -= dmul*tx[k][j];
      } 
    }
  }
  pvt[nrowsmone] = nrowsmone;

  // Forward substitution LY = b
  for (int k = 0; k < nrowsmone; k++) {
    int kbrow = min(nrowsmone - k, tx.bwleft); 
    int pvtk = pvt[k];
    T sb = bb[pvtk];
    if (k != pvtk) swap(bb[k], bb[pvtk]);
    for (int j = 1; j <= kbrow; j++) 
      bb[k+j] -= tx[k+j][-j]*sb;
  }

  // Backward substitution U x = y
  for (int k = nrowsmone; k>= 0; k--) {
    int kb = min(nrowsmone -k, tx.bwrit); 
    for (int j = 1; j <= kb; j++) bb[k] -= tx[k][j]*bb[k+j];
    bb[k] /= tx[k][0];
  }
} // end GaussElimPP()


// BandGauss elimination with partial  pivoting
template<class T>
void BandMtx<T>::GaussElimPP2(Vcr<T>& bb) const {
  if (nrows != bb.size()) 
    error("matrix-vector sizes not match");

// Gauss eliminations with paritial pivoting
// for band matrices. This version interchanges rows 
// in L and U and does not have any advantage since
// it requires more storage than using full matrix
// storage format. A straight forward programming without
// thinking leads to this version.

// This version should never be used. instead use
// BandMtx<T>::GaussElimPP(), which requires more thinking
// to understand.

  BandMtx<T> tx(nrows, nrows-1,
                       min(nrows-1, bwleft+bwrit));
  for (int i = 0; i < nrows; i++) 
    for (int j = - bwleft; j <= bwrit; j++) 
      tx[i][j] = bdmx[i][j];

  Vcr<int> pvt(nrows);   // store pivoting info
  for (int k = 0; k < nrows; k++) pvt[k] = k;

  // LU decomposition with column partial pivoting
  for (int k = 0; k < tx.nrows - 1; k++) {
    int kbrow = min(nrows - 1 - k, tx.bwleft); 
    int kbcol = min(nrows - 1 - k, tx.bwrit); 

    // find the pivot in the k-th column
    int pc = k;
    double aet = abs(tx[k][0]);
    for (int i = 1; i <= kbrow; i++) {
      if (abs(tx[k+i][-i]) > aet) {
        aet = abs(tx[k+i][-i]);
        pc = k + i;
      }
    }
    if (!aet) error("pivot is zero in GaussElimPP");

    // interchange pivot row and k-th row
    if (pc != k) {
      swap(pvt[k], pvt[pc]);
      for (int j = - min(k, tx.bwleft); j <= kbcol; j++) 
        swap(tx[pc][j+k-pc], tx[k][j]);
    }

    // now eliminate column entries below main 
    // diagonal entry at row k
    for (int i = 1; i <= kbrow; i++) { 
      int kpi = k + i;
      if (tx[kpi][-i] != 0) {
        T dmul = tx[kpi][-i]/tx[k][0];
        tx[kpi][-i] = dmul;
        for (int j = 1; j <= kbcol; j++) 
          tx[kpi][j-i] -= dmul*tx[k][j];
      } 
    }
  }

  Vcr<T> rhv(nrows);
  for (int k = 0; k < nrows; k++) rhv[k] = bb[pvt[k]];
  bb = rhv; 

  // Forward substitution LY = Pb
  for (int k = 1; k < tx.nrows; k++) {
    int kb  = min(k, tx.bwleft); 
    for (int j = 1; j <= kb; j++) 
      bb[k] -= tx[k][-j]*bb[k-j];
  }

  // Backward substitution
  for (int k = tx.nrows-1; k>= 0; k--) {
    int kb = min(tx.nrows -1-k, tx.bwrit); 
    for (int j = 1; j <= kb; j++) bb[k] -= tx[k][j]*bb[k+j];
    bb[k] /= tx[k][0];
  }
} // end GaussElimPP2()





// *** definitions for members of class SSMtx

template<class T>
SSMtx<T>::SSMtx(int n, int m, T* ent, int* con, int* dia) {
  nrows = n;
  lenth = m;
  sra = new T [lenth];
  clm = new int [lenth];
  dgn = new int [nrows];

  for (int i =0; i< lenth; i++)  sra[i] = ent[i];
  for (int i =0; i< nrows; i++)  dgn[i] = dia[i];
  for (int i =0; i< lenth; i++)  clm[i] = con[i];
}

template<class T>
SSMtx<T>::SSMtx(int n, int m) {
  nrows = n;
  lenth = m;
  sra = new T [lenth];
  clm = new int [lenth];
  dgn = new int [nrows];

  for (int i =0; i< lenth; i++)  sra[i] = 0;
  for (int i =0; i< nrows; i++)  dgn[i] = 0;
  for (int i =0; i< lenth; i++)  clm[i] = 0;
}

template<class T>
SSMtx<T>::SSMtx(const SSMtx & mat) {   // copy constructor
  nrows = mat.nrows;
  lenth = mat.lenth;
  sra = new T [lenth]; 
  clm = new int [lenth]; 
  dgn   = new int [nrows]; 
  for (int i =  0; i< nrows; i++) dgn[i] = mat.dgn[i];
  for (int i = 0; i < lenth; i++) {
    sra[i] = mat[i]; 
    clm[i] = mat.clm[i]; 
  }
}

template<class T>
SSMtx<T>& SSMtx<T>::operator=(const SSMtx & ssm) {
  if(nrows !=ssm.nrows || lenth != ssm.lenth) error("bad matrix sizes");
  for (int i = 0; i < lenth; i++) {
    sra[i]  = ssm[i];
    clm[i]  = ssm.clm[i];
  }
  for (int i = 0; i < nrows; i++) dgn[i] = ssm.dgn[i]; 
  return *this;
}

template<class T>
Vcr<T> SSMtx<T>::operator*(const Vcr<T> & vec) const {
  if (nrows != vec.size()) error("matrix and vector sizes do not match");
  Vcr<T> tm(nrows);

  tm[0] += sra[0]*vec[0]; 
  for (int j = 1; j < nrows; j++) {
    int m = dgn[j-1]+1;  
    if (clm[m] == 0) tm[0] += sra[m]*vec[j]; 
  }
  for (int i = 1; i < nrows; i++)  {
    for (int j = dgn[i-1] + 1; j <= dgn[i]; j++) 
      tm[i] += sra[j]*vec[clm[j]]; 
    for (int j = i+1; j < nrows; j++) {
      for (int m = dgn[j-1]+1;  m < dgn[j]; m++) 
        if (clm[m] == i) tm[i] += sra[m]*vec[j]; 
    }
  }
  return tm;
}

template<class T>
Vcr<T> SSMtx<T>::preconding(const Vcr<T>& r, int prec) const {
// solve preconditioning matrix:  Pz = r;
// prec: 0 if no preconditioning, 1 if diag prec, 2 if SSOR prec

  int & sz=nrows;
  if (prec == 0) {   // no preconditioning
    return r;
  } else if (prec == 1 ){            // diagonal preconditioning
    Vcr<T> z(nrows);
    for (int i = 0; i < sz; i++)  z[i] = r[i]/sra[dgn[i]];
    return z;
  } else if (prec == 2) {    // symmetric SOR preconditioning
    const T omega = 1;   // SSOR parameter for preconditioning
    Vcr<T> z(nrows);
    Vcr<T> tm(nrows);
    tm[0] = r[0]/sra[0];
    for (int i = 0; i < sz; i++) {
      if (abs(sra[dgn[i]]) <= SMALL) {
        error("SSOR preconditioner did not work");
        break;
      } 
      T sum = 0;
      for (int j = dgn[i-1]+1; j < dgn[i]; j++)  
        sum += omega*sra[j]*tm[clm[j]];
      tm[i] = (r[i] - sum)/sra[dgn[i]];
    }
    for (int i = sz -1; i >= 0; i--) {
      T sum = 0;
      for (int j = i+1; j < sz; j++) {
        for (int m = dgn[j-1]+1;  m < dgn[j]; m++) 
          if (clm[m] == i) sum += omega*sra[m]*z[j];
      }
      z[i] = tm[i] - sum/sra[dgn[i]];
    }
    return z;
  }
}

// *** definitions for members of class SparseMtx

template<class T>
SparseMtx<T>::SparseMtx(int n, int m, T* et, int* cn, int* da) {
  nrows = n;
  lenth = m;
  sra = new T [lenth];
  clm = new int [lenth];
  fnz = new int [nrows +1];

  for (int i =0; i< lenth; i++) {
    sra[i] = et[i];
    clm[i] = cn[i];
  }
  for (int i = 0; i <= nrows; i++)  fnz[i] = da[i];
}

template<class T>
SparseMtx<T>::SparseMtx(int n, int m) {
  nrows = n;
  lenth = m;
  sra = new T [lenth];
  clm = new int [lenth];
  fnz = new int [nrows +1];

  for (int i =0; i< lenth; i++)  {
    sra[i] = 0;
    clm[i] = 0;
  }
  for (int i =0; i <= nrows; i++)  fnz[i] = 0;
}

template<class T>
SparseMtx<T>::SparseMtx(const SparseMtx & mat) {           // copy constructor
  nrows = mat.nrows;
  lenth = mat.lenth;
  sra = new T [lenth]; 
  clm = new int [lenth]; 
  fnz = new int [nrows +1]; 
  for (int i = 0; i < lenth; i++) {
    sra[i] = mat[i]; 
    clm[i] = mat.clm[i]; 
  }
  for (int i =  0; i <= nrows; i++) fnz[i] = mat.fnz[i];
}

template<class T>
SparseMtx<T>& SparseMtx<T>::operator=(const SparseMtx & ssm) {
  if(nrows != ssm.nrows || lenth != ssm.lenth) 
    error("bad matrix sizes in SparseMtx::operator=()");
  for (int i = 0; i < lenth; i++) {
    sra[i]  = ssm[i];
    clm[i]  = ssm.clm[i];
  }
  for (int i = 0; i <= nrows; i++) fnz[i] = ssm.fnz[i]; 
  return *this;
}

template<class T>
Vcr<T> SparseMtx<T>::operator*(const Vcr<T>& vec) const {
  if (nrows != vec.size()) 
    error("matrix and vector sizes do not match in SparseMtx::operator*()");
  Vcr<T> tm(nrows);
  for (int i = 0; i < nrows; i++)  
    for (int j = fnz[i]; j < fnz[i +1]; j++) tm[i] += sra[j]*vec[clm[j]];
  return tm;
}

template<class T>                 // solve Pz = r, return z. (P: preconditioner)
Vcr<T> SparseMtx<T>::preconding(const Vcr<T>& r, int precn) const {
  if (precn == 0) {               // no preconditioning is used
    return r;
  } else if (precn == 1) {        // diagonal preconditioning
    Vcr<T> diag(nrows);           // find diagonal entries
    for (int i = 0; i < nrows; i++) {        // geometrical average, more stable
      for (int j = fnz[i]; j < fnz[i+1]; j++) diag[i] += sra[j]*sra[j];
      diag[i] = sqrt(diag[i]); 
    }

    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) z[i] = r[i]/diag[i];
    return z;
  } else if (precn == 2) {        // SSOR preconditioning
    const T omega = 1;            // SOR parameter for preconditioning

    Vcr<T> diag(nrows);           // find diagonal entries
    for (int i = 0; i < nrows; i++) {        
      for (int j = fnz[i]; j < fnz[i+1]; j++) {
        if (clm[j] == i) diag[i] += sra[j]*sra[j];    // averaging is used
        else diag[i] += omega*omega*sra[j]*sra[j];    // it is more stable
      }
      diag[i] = sqrt(diag[i]);
    }

    Vcr<T> z(nrows);
    for (int i = 0; i < nrows; i++) {           // solve lower triangular system
      T sum = 0;
      for (int j = fnz[i]; j < fnz[i+1]; j++) {
        if (clm[j] < i) sum += omega*sra[j]*z[clm[j]];
      }
      z[i] = (r[i] - sum)/diag[i];
    }
    for (int i = nrows -1; i >= 0; i--) {       // solve upper triangular system
      T sum = 0;
      for (int j = fnz[i]; j < fnz[i+1]; j++) {
        if (clm[j] > i) sum += omega*sra[j]*z[clm[j]];
      }
      z[i] -= sum/diag[i];
    }
    return z;
  } else {
    error("specified preconditioner in SparseMtx::preconding not implemented");
  } 
} // end SparseMtx::preconding()

#endif

//****** end matvec.h  ***************/
