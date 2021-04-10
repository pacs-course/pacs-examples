#include <iosfwd>
#include <ratio> // from c++11 only!
namespace apsc
{
//! A simple class that represent a rational number
class Rational
{
public:
  //! This is also a default constructor.
  /*!  If the constructor is declared explicit to avoid
    implicit conversion from an integer. 
    It is necessary  because we have defined also the conversion
    Rational -> double. If we had not declared it explicit the operation
    r + 1 , with r a Rational, would be ambiguous! It is a.double + double(1) 
    (implicit conversion of Rational and  int to double) or r + Rational(1)
    (implicit convertion int to Rational)? The compiler is not psychic and 
    will emit an error. 
    With explicit constructors r + 1 will always convert in r.double() +
    double(1). If I want to have a rational from the integer I need to
    write r + Rational(1) explicitly (or use static_cast<Rational>).
   */
  explicit Rational(int n=0, int d=1);
  //! Constructor from a constant rational (since C++11).
  /*! Also using a constructor of the class in the initialization list
    is C++11 only. The argument is dummy, we do not need it.
  */
  template <std::intmax_t N, std::intmax_t D>
  Rational(std::ratio<N,D> const &):Rational(N,D){};
  // Copy/move operators not needed: automatic ones are fine.
  /* 
    We might declare explicitly that the automatic
    copy constructor is what we want by writing
    @code  
    Rational(Rational const &)=default;
    @endcode
    and so for the other copy/move operators
  */
  //! Returns the numerator.
  int numerator()const { return M_n;};
  //! Returns the denominator.
  int denominator()const { return M_d;};
  //! converts a Rational to a double.
  operator double() const;
  /** Arithmetic operators (methods) */
 //@{
  Rational &operator +=(Rational const &);
  Rational &operator -=(Rational const &);
  Rational &operator /=(Rational const &);
  Rational &operator *=(Rational const &);
  Rational operator-() const;
  Rational operator+() const {return *this;}
  Rational & operator++();
  Rational operator++(int);
  Rational & operator--();
  Rational operator--(int);
   //@}
  //! Addressing operator
  //!
  //! Not really needed, only to show their use.
  //! I provide only the const version since the non-const version
  //! should return a reference, to allow modifications. But, I cannot
  //! expose the numerator or denominator for changes, since
  //! I would then need to normalize the rational afterwards.
  //!
  //! @param i the index
  //! @result Numerator if i==0, Denominator otherwise.
   int operator[](unsigned int i)const { return i==0u?M_n:M_d;}
  /** Arithmetic operators (friend classes) */
  //@{
  friend Rational operator+(Rational const &,Rational const &);
  friend Rational operator-(Rational const &,Rational const &);
  friend Rational operator*(Rational const &,Rational const &);
  friend Rational operator/(Rational const &,Rational const &);
  friend bool operator<(Rational const&, Rational const &);
  //@}
  //! Streaming operator to output rationals in a nice way.
  friend std::ostream & operator << (std::ostream &, Rational const &);
  //! Streaming operator to input rationals in a nice way.
  friend std::istream & operator >> (std::istream &, Rational &);
private:
  int M_n, M_d;
  //! Helper function to normalize the internal rational.
  void M_normalize();
};

//* Logical operator definition (inlined for efficiency)
//! The less-than operator
inline bool operator<(Rational const& l, Rational const & r)
{
  return (l.M_n*r.M_d) < (r.M_n*l.M_d);
}

//! Defined in terms of <
/*!
  @note For the Rational this is not necessary since the equality operator
  can easily be trasformed in equality between integers. It is only to stress
  that this is the general rule for equality operators.
*/
inline bool operator==(Rational const& l, Rational const & r)
{
  return !(l<r) && ! (r<l);
}
//! Defined in term of < and ==
inline bool operator<=(Rational const& l, Rational const & r)
{
  return (l<r) || (l==r);
}
//! defined in terms of <=
inline bool operator>(Rational const& l , Rational const & r){
  return ! (l<=r);
}
//! defined in terms of <
inline bool operator>=(Rational const& l, Rational const & r)
{
  return !(l<r);
}
}// end namespace
