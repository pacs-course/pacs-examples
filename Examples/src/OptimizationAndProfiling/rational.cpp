#include <iostream>
#include <cstdlib> // for abs()
#include "extendedAssert.hpp"
#include "rational.hpp"

Rational::Rational(int n, int d): M_n(n),M_d(d) {
  PRE_ASSERT(d!=0,"Cannot construct a rational with zero denominator");
  this->M_normalize();}


Rational::Rational(Rational const & r):M_n(r.M_n),M_d(r.M_d)
{this->M_normalize();}

Rational::operator  double() const
{return static_cast<double>(M_n)/static_cast<double>(M_d);}

Rational Rational::operator-() // unary minus overloading
{
  return Rational(-this->M_n,this->M_d);
}

Rational Rational::operator+() // unary Plus overloading
{
  return Rational(*this);
}


Rational & Rational::operator ++()
{
  M_n+=M_d;
  return *this;
}

Rational  Rational::operator ++(int)
{
  Rational temp(*this);
  this->operator++();
  return temp;
}

Rational & Rational::operator --()
{
  M_n-=M_d;
  return *this;
}

Rational  Rational::operator --(int)
{
  Rational temp(*this);
  this->operator--();
  return temp;
}


Rational &
Rational::operator +=(Rational const & r)
{
  M_n =M_n*r.M_d+ r.M_n*M_d;
  M_d *=r.M_d;
  M_normalize();
  return *this;
}

Rational operator +(Rational const &l, Rational const & r)
{
  Rational temp(l);
  temp+=r;
  return temp;
}




Rational &
Rational::operator -=(Rational const & r)
{
  M_n =M_n*r.M_d-r.M_n*M_d;
  M_d *=r.M_d;
  M_normalize();
  return *this;
}

Rational operator -(Rational const &l, Rational const & r)
{
  Rational temp(l);
  temp-=r;
  return temp;
}

Rational &
Rational::operator *=(Rational const & r)
{
  M_n *=r.M_n;
  M_d *=r.M_d;
  M_normalize();
  return *this;
}

Rational operator *(Rational const &l, Rational const & r)
{
  Rational temp(l);
  temp*=r;
  return temp;
}

Rational &
Rational::operator /=(Rational const & r)
{
  M_n *=r.M_d;
  M_d *=r.M_n;
  M_normalize();
  return *this;
}

Rational operator /(Rational const &l, Rational const & r)
{
  Rational temp(l);
  temp/=r;
  return temp;
}



std::ostream & operator << (std::ostream & str, Rational const & r)
{
  if (r.M_d==1) str<< r.M_n;
  else
    if (int d=r.M_n / r.M_d)
      str<<d<<"+" << r.M_n%r.M_d << '/' << r.M_d;
    else
      str<< r.M_n << '/' << r.M_d;
  return str;
}


void Rational::M_normalize()
{
  INV_ASSERT(M_d!=0,"Zero denominator in Rational");
  // handle particular cases
  if (M_d==0) return; // 0 Denominator, do nothing!

  if(M_n==0){
    M_d=(M_d>0?1:-1); // M_d=sign(M_d)
    return;
  }
  // Fix the sign
  bool negative=(M_n*M_d)<0;
  
  // I use the absolute values here
  M_n = std::abs(M_n);
  M_d = std::abs(M_d);
  
  // Euclid's algorithm...
  int a, b, r;
  if (M_d > M_n){
    a = M_d;
    b = M_n;
  }
  else{
    a = M_n;
    b = M_d;
  }
  r = 1;
  while (r > 0 && b > 0){
    r = a % b;
    a = b;
    b = r;
  }
  M_n/=a;
  M_d/=a;
  // Get back the right sign
  if (negative) M_n=-M_n;
}


