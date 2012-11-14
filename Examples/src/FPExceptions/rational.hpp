#include "extendedAssert.hpp"
#include "myexceptions.hpp"
#include <iosfwd>
class Rational
{
public:
  Rational(int n=0, int d=1);
  Rational(Rational const &);
  int numerator()const { return M_n;};
  int denominator()const { return M_d;};
  operator double() const;
  Rational &operator +=(Rational const &);
  Rational &operator -=(Rational const &);
  Rational &operator /=(Rational const &);
  Rational &operator *=(Rational const &);
  Rational operator-() const;
  Rational operator+() const;
  Rational & operator++();
  Rational operator++(int);
  Rational & operator--();
  Rational operator--(int);
  friend Rational operator+(Rational const &,Rational const &);
  friend Rational operator-(Rational const &,Rational const &);
  friend Rational operator*(Rational const &,Rational const &);
  friend Rational operator/(Rational const &,Rational const &);
  friend std::ostream & operator << (std::ostream &, Rational const &);
private:
  int M_n, M_d;
  void M_normalize(); // Normalize the rational
};
  Rational operator+(Rational const &,Rational const &);
  Rational operator-(Rational const &,Rational const &);
  Rational operator*(Rational const &,Rational const &);
  Rational operator/(Rational const &,Rational const &);
  std::ostream & operator << (std::ostream &, Rational const &);



