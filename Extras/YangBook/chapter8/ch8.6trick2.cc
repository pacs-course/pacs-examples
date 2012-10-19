// Barton-Nackman trick on two examples

// example on suming diagonal entries of full and symmetric matrices

#include <iostream>

template<class T> class Matrix {
public:
 int dimn;
 T& ReferToDerived() {
   return static_cast<T&>(*this);
 }

 double& operator()(int i, int j) {
   return ReferToDerived()(i,j);
 }
};

// define function `sum' that calls operator ()
template<class T> double sum(Matrix<T>& A) {
  double d = 0;
  for (int i = 0; i < A.dimn; i++) { 
    for (int j = 0; j < A.dimn; j++) 
      d += A(i,j);      // call operator ()
  }
  return d;
}

class FullMtx: public Matrix<FullMtx> {
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
  double* operator[](int i) { return mx[i]; }
  double& operator()(int i, int j) { return mx[i][j]; }
};

class SymmetricMtx: public Matrix<SymmetricMtx> {
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
  double* operator[](int i) { return mx[i]; }
  double& operator()(int i, int j) { 
    if (i >= j ) return mx[i][j]; 
    else return mx[j][i];          // due to symmetry
  }
};

int main() {
  FullMtx A(2);
  A[0][0] = 5; A[0][1] = 3; A[1][0] = 3; A[1][1] = 6;
  std::cout << "sum of full matrix A = " << sum(A) << '\n';

  SymmetricMtx S(2);   // just assign lower triangular part
  S[0][0] = 5; S[1][0] = 3; S[1][1] = 6;
  std::cout << "sum of symmetric matrix S = " << sum(S) << '\n';
}
