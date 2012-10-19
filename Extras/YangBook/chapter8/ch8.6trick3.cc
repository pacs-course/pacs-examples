// somethign better than Barton-Nackman trick 

#include <iostream>

class Vcr {   /* a vector class */  };

template<class Matrix> 
void CG(const Matrix& m, const Vcr& v) { 

  // CG is not fully defined here
  Vcr w = m*v;    // call matrix vector multiply
  std::cout << "calling CG\n";

}

template<class Matrix> 
void GMRES(const Matrix& m, const Vcr& v) { 

  // full definition of GMRES is omited here
  Vcr w = m*v;    // call matrix vector multiply
  std::cout << "calling GMRES\n";

}

class FullMatrix {
//   encapsulate storage information for full matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (full) matrix vector multiply
    std::cout << "calling full matrix vector multiply\n";

    return Vcr();
  }
};

class BandMatrix {
//   encapsulate storage information for band matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (band) matrix vector multiply
    std::cout << "calling band matrix vector multiply\n";

    return Vcr();
  }
};

class SparseMatrix {
//   encapsulate storage information for sparse matrix
public:
  Vcr operator*(const Vcr& v) const {
    // define (sparse) matrix vector multiply
    std::cout << "calling sparse matrix vector multiply\n";

    return Vcr();
  }
};

void h() {
  Vcr v;
  FullMatrix A;
  CG(A, v);           // Calling CG() on full matrix
  GMRES(A, v);        // Calling GMRES() on full matrix

  BandMatrix B;
  CG(B, v);           // Calling CG() on band matrix
  GMRES(B, v);        // Calling GMRES() on full matrix

  SparseMatrix S;
  CG(S, v);           // Calling CG() on sparse matrix
  GMRES(S, v);        // Calling GMRES() on full matrix
}

// example two

template<class Matrix> double sum(Matrix& m) {
  double d = 0;
  for (int i = 0; i < m.dimn; i++)
    for (int j = 0; j < m.dimn; j++) d += m.entry(i,j);
  return d;
}

class FullMtx {
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

  int dimn;

  double& operator()(int i, int j) { return mx[i][j]; }

  double entry(int i, int j) {
    return (*this)(i,j);
  }
};

class SymmetricMtx {
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

  int dimn;

  double& operator()(int i, int j) { 
    if (i >= j ) return mx[i][j]; 
    else return mx[j][i];          // due to symmetry
  }

  double entry(int i, int j) {
    return (*this)(i,j);
  }
};

void g() {
  FullMtx A(2);
  A(0,0) = 5; A(0,1) = 3; A(1,0) = 3; A(1,1) = 6;
  std::cout << "sum of full matrix A = " << sum(A) << '\n';

  SymmetricMtx S(2);   // just assign lower triangular part
  S(0,0) = 5; S(1,0) = 3; S(1,1) = 6;
  std::cout << "sum of symmetric matrix S = " << sum(S) << '\n';
}

int main() {
 h();
 g();
}
