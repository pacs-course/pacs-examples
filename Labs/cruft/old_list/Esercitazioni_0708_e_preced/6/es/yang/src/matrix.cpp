#include "matrix.hpp"

namespace LinearAlgebra {
  
  Matrix::Matrix(int size1, int size2, double** a) {
    M_super = new double* [M_size1 = size1];
    M_size2 = size2;
    for(int i = 0; i < M_size1; i++) {
      M_super[i] = new double [M_size2];
      for(int j = 0; j < M_size2; j++) M_super[i][j] = a[i][j];
    }
  }

  Matrix::Matrix(int size1, int size2, double s) {
    M_super = new double* [M_size1 = size1];
    M_size2 = size2;
    for(int i = 0; i < M_size1; i++) {
      M_super[i] = new double [M_size2];
      for(int j = 0; j < M_size2; j++) M_super[i][j] = s;
    }
  }

  Matrix::Matrix(const Matrix& m) {
    M_super = new double* [M_size1 = m.size1()];
    M_size2 = m.size2();
    for(int i = 0; i < M_size1; i++) {
      M_super[i] = new double [M_size2];
      for(int j = 0; j < M_size2; j++) M_super[i][j] = m[i][j];
    }
  }

  Matrix::~Matrix(){
    for(int i = 0; i < M_size1; i++) delete[] M_super[i];
    delete[] M_super;
  }

  Matrix& Matrix::operator=(const Matrix& m) {
    if(this != &m) { // Beware of self-assignment
      if( M_size1 != m.size1() || M_size2 != m.size2())
	std::cerr << "[Matrix::operator=] ERROR: bad matrix size"
		  << std::endl;
      for(int i = 0; i < M_size1; i++)
	for(int j = 0; j < M_size2; j++)
	  M_super[i][j] = m[i][j];
    }
    return *this;
  }

  Matrix& Matrix::operator+=(const Matrix& m) {
    if( M_size1 != m.size1() || M_size2 != m.size2() )
      std::cerr << "[Matrix::operator=] ERROR: bad matrix size"
		<< std::endl;
    for(int i = 0; i < M_size1; i++)
      for(int j = 0; j < M_size2; j++)
	M_super[i][j] += m[i][j];
    return *this;
  }

  Matrix& Matrix::operator-=(const Matrix& m) {
    if( M_size1 != m.size1() || M_size2 != m.size2() )
      std::cerr << "[Matrix::operator=] ERROR: bad matrix size"
		<< std::endl;
    for(int i = 0; i < M_size1; i++)
      for(int j = 0; j < M_size2; j++)
	M_super[i][j] -= m[i][j];
    return *this;
  }

  Matrix operator+(const Matrix& m1, const Matrix& m2) {
    if( m1.size1() != m2.size1() || m1.size2() != m2.size2() )
      std::cerr << "[Matrix::operator=] ERROR: bad matrix size"
		<< std::endl;
    Matrix sum( m1 );
    sum += m2;
    return sum;
  }

  Matrix operator-(const Matrix& m1, const Matrix& m2) {
    if( m1.size1() != m2.size1() || m1.size2() != m2.size2() )
      std::cerr << "[Matrix::operator=] ERROR: bad matrix size"
		<< std::endl;
    Matrix diff( m1 );
    diff -= m2;
    return diff;
  }

  Vector Matrix::operator*(const Vector& v) const {
    if(M_size2 != v.size())
      std::cerr << "[Matrix::operator*(Vector)] ERROR:"
		<< " matrix and vector sizes don't match"
		<< std::endl;
    Vector mv(M_size1);
    for(int i = 0; i < M_size1; i++)
      for(int j = 0; j < M_size2; j++)
	mv[i] += M_super[i][j] * v[j];
    return mv;
  }

  std::ostream& operator<<(std::ostream& ostr, const Matrix& m) {
    ostr << "[" << m.size1() << ", " << m.size2() << "](" << std::flush;
             
    for(int i = 0; i < m.size1() - 1; i++) {
      ostr << "(" << std::flush;
      for(int j = 0; j < m.size2() - 1; j++) {
	ostr << m[i][j] << ", " << std::flush;
      }
      ostr << m[i][m.size2() - 1] << "), " << std::flush;
    }
    ostr << "(" << std::flush;
    for(int j = 0; j < m.size2() - 1; j++)
      ostr << m[m.size1() - 1][j] << ", " << std::flush;

    ostr << m[m.size1() - 1][m.size2() - 1] << "))" << std::flush;

    return ostr;
  }

  Vector diag(const Matrix& A) {
    int n = std::min(A.size1(), A.size2());
    Vector v(n, 0.);
    for(int i = 0; i < n; i++)
      v[i] = A(i, i);
    return v;
  }

  Vector operator*(const Vector& v, const Matrix& A) {
    if(A.size1() != v.size())
      std::cerr << "[Vector::operator*(Matrix)] ERROR: "
		<< "matrix and vector sizes don't match"
		<< std::endl;
    Vector vm(A.size2());
    for(int j = 0; j < A.size2(); j++)
      for(int i = 0; i < A.size1(); i++)
	vm[j] += v[i] * A(i, j);
    return vm;
  }

  double norm_fro(const Matrix& A) {
    double norm = 0.;
    for(int i = 0; i < A.size1(); i++)
      for(int j = 0; j < A.size2(); j++)
	norm += pow(A(i, j), 2);
    return sqrt(norm);

  }

  double norm_psi(const Matrix& A) {
    double norm = 0.;
    for(int i = 0; i < A.size1(); i++)
      for(int j = 0; j < A.size2(); j++)
	if(i != j) norm += pow(A(i, j), 2);
    return sqrt(norm);

  }
}
