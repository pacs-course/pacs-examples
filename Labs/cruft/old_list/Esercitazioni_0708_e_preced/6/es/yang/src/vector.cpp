#include "vector.hpp"

namespace LinearAlgebra {
  Vector::Vector(int n, double* a) {
    M_super = new double [M_size = n];
    for(int i = 0; i < M_size; i++) M_super[i] = *(a + i);
  }

  Vector::Vector(int n, double s) {
    M_super = new double [M_size = n];
    for(int i = 0; i < M_size; i++) M_super[i] = s;
  }

  Vector::Vector(const Vector& v) {
    M_super = new double [M_size = v.size()];
    for(int i = 0; i < M_size; i++) M_super[i] = v[i];
  }

  Vector& Vector::operator=(const Vector& v) {
    if(this != &v) { // Beware of self-assignment
      if(M_size != v.size()) 
	std::cerr << "[Vector::operator=] ERROR: bad vector size" 
		  << std::endl;
      for(int i = 0; i < M_size; i++) M_super[i] = v[i];
    }
    return *this;
  }

  Vector& Vector::operator+=(const Vector& v) {
    if(M_size != v.size())
      std::cerr << "[Vector::operator+=] ERROR: bad vector size" 
		<< std::endl;
    for(int i = 0; i < M_size; i++) M_super[i] += v[i];
    return *this;
  }

  Vector& Vector::operator-=(const Vector& v) {
    if(M_size != v.size())
      std::cerr << "[Vector::operator-=] ERROR: bad vector size" 
		<< std::endl;
    for(int i = 0; i < M_size; i++) M_super[i] -= v[i];
    return *this;
  }

  Vector& Vector::operator/=(double s) {
    if(s == 0.)
      std::cerr << "[Vector::operator/=] ERROR: division by 0"
		<< std::endl;
    for(int i = 0; i < M_size; i++) M_super[i] /= s;
    return *this;
  }

  inline Vector operator+(const Vector& v) {
    return v;
  }

  inline Vector operator-(const Vector& v) {
    return Vector(v.size()) - v;
  }

  Vector operator+(const Vector& v1, const Vector& v2) {
    if(v1.size() != v2.size())
      std::cerr << "[Vector::operator+] ERROR: bad vector size" 
		<< std::endl;
    Vector sum( v1 );
    sum += v2;
    return sum;
  }

  Vector operator-(const Vector& v1, const Vector& v2) {
    if(v1.size() != v2.size())
      std::cerr << "[Vector::operator-] ERROR: bad vector size" 
		<< std::endl;
    Vector diff( v1 );
    diff -= v2;
    return diff;
  }

  Vector operator*(double s, const Vector& v) {
    Vector sv(v.size());
    for(int i = 0; i < v.size(); i++) sv[i] = s * v[i];
    return sv;
  }

  Vector operator*(const Vector& v, double s) {
    Vector sv(v.size());
    for(int i = 0; i < v.size(); i++) sv[i] = s * v[i];
    return sv;
  }

  Vector operator/(const Vector& v, double s) {
    Vector sv(v.size());
    for(int i = 0; i < v.size(); i++) sv[i] = v[i] / s;
    return sv;
  }

  double dot(const Vector& v1, const Vector& v2) {
    if(v1.size() != v2.size())
      std::cerr << "[dot] ERROR: bad vector size" 
		<< std::endl;
    double s = v1[0] * v2[0];
    for(int i = 1; i < v1.size(); i++) s += v1[i] * v2[i];
    return s;
  }

  std::ostream& operator<<(std::ostream& ostr, const Vector& v) {
    ostr << "[" << v.size() << "](" << std::flush;
    for(int i = 0; i < v.size() - 1; i++)
      ostr << v[i] << ", " << std::flush;
    ostr << v[v.size() - 1] << ")" << std::flush;
    return ostr;
  }

  double norm_2(const Vector& v) {
    double norm = 0.;
    for(int i = 0; i < v.size(); i++)
      norm += pow(v[i], 2);
    return sqrt(norm);
  }

  double norm_inf(const Vector& v) {
    double norm = v[0];
    for(int i = 0; i < v.size(); i++)
      if(v[i] > norm) norm = v[i];
    return norm;
  }
}
