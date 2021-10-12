#include "extendedAssert.hpp"
#include <iosfwd>
//! A Class for rational number.
class Rational
{
public:
  Rational(int n = 0, int d = 1);
  Rational(Rational const &);
  INLINE int
  numerator()
  {
    return M_n;
  };
  INLINE int
  denominator()
  {
    return M_d;
  };
                       operator double() const;
  Rational &           operator+=(Rational const &);
  Rational &           operator-=(Rational const &);
  Rational &           operator/=(Rational const &);
  Rational &           operator*=(Rational const &);
  Rational             operator-();
  Rational             operator+();
  Rational &           operator++();
  Rational             operator++(int);
  Rational &           operator--();
  Rational             operator--(int);
  friend Rational      operator+(Rational const &, Rational const &);
  friend Rational      operator-(Rational const &, Rational const &);
  friend Rational      operator*(Rational const &, Rational const &);
  friend Rational      operator/(Rational const &, Rational const &);
  friend std::ostream &operator<<(std::ostream &, Rational const &);

private:
  int  M_n, M_d;
  void M_normalize(); // Normalize the rational
};
