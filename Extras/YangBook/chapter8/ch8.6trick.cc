// Barton-Nackman trick on two examples

// example one

#include <iostream>

class Vcr {   /* a vector class */  };

template<class T> class Matrix {      // base class

  // T will refer to a derived class when this template is instantiated
  const T& ReferToDerived() const {
    return static_cast<const T&>(*this);
  }

public:

  // refer operation to derived class
  Vcr operator*(const Vcr& v) const {
    return ReferToDerived() * v;        
  }

  // define common functionality in base class that can
  // be called on derived classes 
  void CG(const Vcr& v) { // CG is not fully defined here
    Vcr w = (*this)*v;    // call matrix vector multiply
    std::cout << "calling CG" << std::endl;
  }
  void GMRES(const Vcr& v) {    // definition omited here
    Vcr w = (*this)*v;          // matrix vector multiply
    std::cout << "calling GMRES" << std::endl;
  }
};

class FullMatrix: public Matrix<FullMatrix> {
//   encapsulate storage information for full matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (full) matrix vector multiply
    std::cout << "calling full matrix vector multiply" << std::endl;

    return Vcr();
  }
};

class BandMatrix: public Matrix<BandMatrix> {
//   encapsulate storage information for band matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (band) matrix vector multiply
    std::cout << "calling band matrix vector multiply" << std::endl;

    return Vcr();
  }
};

class SparseMatrix: public Matrix<SparseMatrix> {
//   encapsulate storage information for sparse matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (sparse) matrix vector multiply
    std::cout << "calling sparse matrix vector multiply" << std::endl;

    return Vcr();
  }
};

void f() {
  Vcr v;
  FullMatrix A;
  A.CG(v);           // Calling CG() on full matrix
  A.GMRES(v);        // Calling GMRES() on full matrix

  BandMatrix B;
  B.CG(v);           // Calling CG() on band matrix
  B.GMRES(v);        // Calling GMRES() on full matrix

  SparseMatrix S;
  S.CG(v);           // Calling CG() on sparse matrix
  S.GMRES(v);        // Calling GMRES() on full matrix
}


// example two 

template<class T> class Mtx {    // base matrix

private:
  // refer to derived class
  T& ReferToDerived() {   
    return static_cast<T&>(*this);
  }

  // entry() uses features of derived class
  double& entry(int i, int j) {
    return ReferToDerived()(i,j);
  }
protected:
  int dimn;           // dimension of matrix       
public:
  // define common functionality in base class that can
  // be called on derived classes 
  double sum() {      // sum all entries
    double d = 0;
    for (int i = 0; i < dimn; i++)
      for (int j = 0; j < dimn; j++) d += entry(i,j);
    return d;
  }
};

class FullMtx: public Mtx<FullMtx> {
  double** mx;
public:
  FullMtx(int n) {
    dimn = n;
    mx = new double* [dimn];
    for (int i=0; i<dimn; i++) mx[i] = new double [dimn];
    for (int i=0; i<dimn; i++)   
      for (int j=0; j<dimn; j++) 
        mx[i][j] = 0;              // initialization
  }
  double& operator()(int i, int j) { return mx[i][j]; }
};

class SymmetricMtx: public Mtx<SymmetricMtx> {
  // store only lower triangular part to save memory
  double** mx;
public:
  SymmetricMtx(int n) {
    dimn = n;
    mx = new double* [dimn];
    for (int i=0; i<dimn; i++) mx[i] = new double [i+1];
    for (int i=0; i<dimn; i++)   
      for (int j = 0; j <= i; j++) 
        mx[i][j] = 0;              // initialization
  }
  double& operator()(int i, int j) { 
    if (i >= j ) return mx[i][j]; 
    else return mx[j][i];          // due to symmetry
  }
};

void g() {
  FullMtx A(2);
  A(0,0) = 5; A(0,1) = 3; A(1,0) = 3; A(1,1) = 6;
  std::cout << "sum of full matrix A = " << A.sum() << std::endl;

  SymmetricMtx S(2);   // just assign lower triangular part
  S(0,0) = 5; S(1,0) = 3; S(1,1) = 6;
  std::cout << "sum of symmetric matrix S = " << S.sum() << std::endl;
}

int main() {
 f();
 g();
}
