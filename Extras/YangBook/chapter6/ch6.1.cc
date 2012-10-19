#include <iostream>
#include <cmath>

class Cmpx {      // class for complex numbers
private:
  double re;      // real part of a complex number
  double im;      // imaginal part of a complex number
public:
  Cmpx(double x = 0, double y = 0) { re =x; im =y; }  // constructor
  Cmpx& operator+=(Cmpx);                          // operator +=, eg z1 +=  z2
  Cmpx& operator-=(Cmpx);                          // operator -=, eg z1 -=  z2
  Cmpx& operator++();                              // prefix,  z1 = ++z
  Cmpx operator++(int);                            // postfix, z1 =  z++

//  Cmpx operator-();                                // operator -, eg z1 = - z2
//  Cmpx operator+();                                // operator +, eg z1 = + z2
//  Cmpx operator-(Cmpx);                            // binary -, eg z = z1 - z2
//  Cmpx operator+(Cmpx);                            // binary +, eg z = z1 + z2

  friend Cmpx operator*(Cmpx, Cmpx);               // binary *, z = z1 * z2
  friend std::ostream& operator<<(std::ostream&, Cmpx);      // operator <<
  friend std::istream& operator>>(std::istream&, Cmpx&);     // operator >> 
};

// these operators can also be conveniently defined as functions.
Cmpx operator+(Cmpx);                   // unary +, eg z1 = + z2
Cmpx operator-(Cmpx);                   // unary -, eg z1 = - z2
Cmpx operator+(Cmpx, Cmpx);             // binary +, z = z1 + z2
Cmpx operator-(Cmpx, Cmpx);             // binary -, z = z1 - z2

// ***********  definitions of Cmpx
inline Cmpx& Cmpx::operator++() {        // ++z
  re++;
  im++;
  return *this;
}

inline Cmpx Cmpx::operator++(int i) {   // z++
  Cmpx temp = *this;
  re++;
  im++;
  return temp;
}


inline Cmpx operator+(Cmpx z) {
  return z;
}

inline Cmpx operator-(Cmpx z) {
  return 0 - z;
}

/*
inline Cmpx Cmpx::operator+() {
  return *this;
}

inline Cmpx Cmpx::operator-() {
  return Cmpx(- re, - im);
}
*/

Cmpx& Cmpx::operator+=(Cmpx z) {
  re += z.re;
  im += z.im;
  return *this;
}

Cmpx& Cmpx::operator-=(Cmpx z) {
  re -= z.re;
  im -= z.im;
  return *this;
}

/*
Cmpx Cmpx::operator+(Cmpx z) {                  // binary +, member fcn
//  Cmpx temp = *this;
//  return temp += z;
  return z += *this;
}

Cmpx Cmpx::operator-(Cmpx z) {                  // binary -, member fcn
  Cmpx temp = *this;
  return temp -= z;
}
*/


Cmpx operator+(Cmpx a, Cmpx b) {                  // binary +, eg z = z1 + z2
  return a += b;  
}

Cmpx operator-(Cmpx a, Cmpx b) {                  // binary -, eg z = z1 - z2
  return a -= b;  
}

Cmpx operator*(Cmpx a, Cmpx b) {                  // binary *, eg z = z1 * z2
  return Cmpx(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);  
}

std::ostream& operator<<(std::ostream& s, Cmpx z) {
  s << "(" << z.re << ", " << z.im << ")";
  return s;
}

std::istream& operator>>(std::istream& s, Cmpx& z) {
  s >> z.re >> z.im;
  return s;
}

int main() {
  Cmpx a(1,1);
  Cmpx b = a;
  b = a + 10;
  std::cout << "a = " << a << "\n";
  Cmpx c = a + b;
  std::cout << "a = " << a << "\n";
  std::cout << "c = " << c << "\n";
  c -=  b;
  std::cout << "c = " << c << "\n";
  c = b;
  c = + b;
  c = a + b*c;
  std::cout << "c = " << c << "\n";

  Cmpx d;
  std::cout << "type two numbers\n";
  std::cin >> d;
  std::cout << "d = " << d << "\n";
  Cmpx e(3.0);
  std::cout << e << "\n";

  Cmpx f = + e;
  std::cout << f << "\n";
  Cmpx g = - e;
  std::cout << g << "\n";

  Cmpx aa = Cmpx(5, 0);
  Cmpx bb = ++aa;
  Cmpx cc = aa++;
  std::cout << "bb = " << bb << "\n";
  std::cout << "cc = " << cc << "\n";
  std::cout << "aa = " << aa << "\n";
}
