/******************** matvec.cc   ***************************
 * This file contains definition of a complete specialization
 * Vcr< complex< double> >. It contains

    ~ add, subtract, multiply complex vectors
    ~ norm and dot product of complex vectors

 *                        written by Daoqi Yang
 *                        last modified September 28, 1998.
 **********************************************************/

#include "matvec.h"

// *** definitions for members in class Vcr< complex<double> >.

Vcr< complex<double> >::Vcr(int n, complex<double>* abd) {
  vr = new complex<double>  [lenth =n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd +i);
}

Vcr<complex<double> >::Vcr(int n, complex<double>  a) {
  vr = new complex<double>  [lenth =n]; 
  for (int i = 0; i < lenth; i++)  vr[i] = a;
}

Vcr<complex<double> >::Vcr(const Vcr & vec) {
  vr = new complex<double>  [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec.vr[i]; 
}

void Vcr<complex<double> >::reset(complex<double>  a) {
 for (int i = 0; i < lenth; i++)  vr[i] = a; 
}

Vcr<complex<double> >& Vcr<complex<double> >::operator=(const Vcr& vec) {
  if (this != &vec) {
    if (lenth != vec.lenth ) error("bad vector sizes in vector assignment");
    for (int i = 0; i < lenth; i++) vr[i] = vec.vr[i];
  }
  return *this;
}

Vcr<complex<double> > & Vcr<complex<double> >::operator+=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes in vector +=");
  for (int i = 0; i < lenth; i++) vr[i] += vec.vr[i];
  return *this;
}

Vcr<complex<double> > & Vcr<complex<double> >::operator-=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes in vector -=");
  for (int i = 0; i < lenth; i++) vr[i] -= vec.vr[i];
  return *this;
}

Vcr<complex<double> > operator+(const Vcr<complex<double> >& vec) {  
  return vec;                                      // usage: vec1 = + vec2;
}
 
Vcr<complex<double> > operator-(const Vcr<complex<double> >& vec) {    
  Vcr<complex<double> > minus = vec;               // usage: vec1 = - vec2;
  for (int i = 0; i < vec.lenth; i++) minus[i] = - vec[i];
  return minus;
}

Vcr<complex<double> > operator+(const Vcr<complex<double> >& v1, 
        const Vcr<complex<double> >& v2) {         // v=v1+v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vector addition");
  Vcr<complex<double> > sum = v1; 
  sum += v2;
  return sum;
}

Vcr<complex<double> > operator-(const Vcr<complex<double> >& v1, 
      const Vcr<complex<double> > & v2) {          // v=v1-v2
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vector subtraction");
  Vcr<complex<double> > sum = v1; 
  sum -= v2;
  return sum;
}

ostream &  operator<<(ostream & s, const Vcr<complex<double> > & vec ) {
  for (int i =0; i < vec.lenth; i++ ) {
    s << vec[i] << "  ";
    if (i%10 == 9) s << "\n";
  }
  return s;
}

double  Vcr<complex<double> >::twonorm() const {
  double  nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    double  vi = abs(vr[i]);
    if (nm < 100) nm = sqrt(nm*nm + vi*vi);
    else {                  // to avoid overflow for fn "sqrt" when nm is large
      double  tm = vi/nm;
      nm *= sqrt(1.0 + tm*tm);
    }
  } 
  return nm;
}

double Vcr<complex<double> >::maxnorm() const {
  double  nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    double  vi = abs(vr[i]);
    if (nm < vi) nm = vi;
  }
  return nm;
}

complex<double>  dot(const Vcr<complex<double> > & v1, 
    const Vcr<complex<double> > & v2) {
  if (v1.lenth != v2.lenth ) error("bad vector sizes in vector dot product");
  complex<double>  tm = v1[0]*conj(v2[0]);
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*conj(v2[i]);
  return tm;
}

Vcr<complex<double> > operator*(complex<double>  scalar, 
     const Vcr<complex<double> > & vec) {
  Vcr<complex<double> > tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = scalar*vec[i]; 
  return tm;
}

Vcr<complex<double> > operator*(const Vcr<complex<double> >& vec, 
       complex<double>  scalar) {
  Vcr<complex<double> > tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = scalar*vec[i]; 
  return tm;
}

Vcr<complex<double> > operator*(const Vcr<complex<double> > & vec1, 
      const Vcr<complex<double> > & vec2) {
  if (vec1.lenth != vec2.lenth ) error("bad vector sizes in vector mulitply");
  Vcr<complex<double> > tm(vec1.lenth);
  for (int i = 0; i < vec1.lenth; i++) tm[i] = vec1[i]*vec2[i]; 
  return tm;
}

Vcr<complex<double> > operator/(const Vcr<complex<double> > & vec, 
      complex<double>  scalar) {
  if (abs(scalar) == 0) error("divisor is zero in vector-scalar division");
  Vcr< complex<double> > tm(vec.lenth);
  for (int i = 0; i < vec.lenth; i++) tm[i] = vec[i]/scalar; 
  return tm;
}

complex<double> dot(complex<double>* a, complex<double>* b, int k) {
  complex<double> init = 0;
  for (int i = 0; i < k; i++) init += *a++ * conj(*b++);
  return init;
}


