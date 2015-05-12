#include <iostream>
#include <ios> // for manipulators
#include <cstdlib> // for abs()
#include <string> 
#include <sstream> // stringstream
#include "extendedAssert.hpp"
#include "rational.hpp"

Rational::Rational(int n, int d): M_n(n),M_d(d) {this->M_normalize();}


// Not needed
//Rational::Rational(Rational const & r):M_n(r.M_n),M_d(r.M_d)
//{this->M_normalize();}

Rational::operator  double() const
{
return static_cast<double>(M_n)/static_cast<double>(M_d);
}

Rational Rational::operator-()const // unary minus overloading
{
  return Rational(-this->M_n,this->M_d);
}

Rational Rational::operator+()const // unary Plus overloading
{
  return *this;
}


Rational & Rational::operator ++()
{
  M_n+=M_d;
  return *this;
}

Rational  Rational::operator ++(int i)
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

Rational  Rational::operator --(int i)
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

bool operator<(Rational const& l, Rational const & r)
{
  auto al=l.M_n*r.M_d;
  auto ar=r.M_n*l.M_d;
  return al < ar;
}
bool operator<=(Rational const& l, Rational const & r)
{
  auto al=l.M_n*r.M_d;
  auto ar=r.M_n*l.M_d;
  return al <= ar;
}
bool operator>(Rational const& l , Rational const & r){
  return ! (l<=r);
}
bool operator>=(Rational const& l, Rational const & r)
{
  return !(l>r);
}
bool operator==(Rational const& l, Rational const & r)
{
  return !(l<r) && ! (r<l);
}


std::ostream & operator << (std::ostream & str, Rational const & r)
{
  if (r.M_d==1) str<< r.M_n;
  else
    if (int d=r.M_n / r.M_d)
      str<<d<< std::showpos<<r.M_n%r.M_d << '/' << 
	std::noshowpos<<r.M_d;
    else
      str<< r.M_n << '/' << r.M_d;
  return str;
}

/*!
  A rather simple input stream. We impose strong restrictions:
  the rational can be given only in the form a/b with a and b integers,
  or in the form a, with a an integer. Blank characters are separators.
  Form a + b/c Is NOT allowed.
 */
std::istream & operator >> (std::istream & str, Rational & r){
  std::string tmp;
  int n,d;
  // Read from stream up to end of line or space char.
  std::getline(str,tmp,' ');
  // Look if we have /
  std::size_t found=tmp.find('/');
  bool haveslash=found != std::string::npos;
  // Get numerator
  std::istringstream s(tmp.substr(0,found));
  s>>n;
  if(s.fail()){
    str.setstate(std::ios::failbit);
    return str;
  }
  // We have a denominator!
  if(haveslash)
    {
      std::istringstream t(tmp.substr(found+1,tmp.size()-found));
      t>>d;
      if(t.fail()){
	str.setstate(std::ios::failbit);
	return str;
      }
    }
    else
      d=1;
  
  r=Rational(n,d);
  /* A faster alternative to the previous statement
     accessing private members of Rational directly
     r.M_d=d;
     r.M_n=n;
     r.M_normalize()
  */
  return str;
}


void Rational::M_normalize()
{
  ASSERTM(M_d!=0,"Zero denominator in Rational");
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


