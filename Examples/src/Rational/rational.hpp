#include <iosfwd>
#include <ratio> // from c++11 only!
#if __cplusplus >= 202002L
#include <compare> // for c++20 style comparison operators
#endif
#include "hashCombine.hpp" // for the hash function
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
  explicit  Rational(int n = 0, int d = 1);
  //! Constructor from a constant rational (since C++11).
  /*! Also using a constructor of the class in the initialization list
    is C++11 only. The argument is dummy, we do not need it.
  */
  template <std::intmax_t N, std::intmax_t D>
  Rational(std::ratio<N, D> const &) : Rational(N, D){};
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
  int
  numerator() const
  {
    return M_n;
  };
  //! Returns the denominator.
  int
  denominator() const
  {
    return M_d;
  };
  //! converts a Rational to a double.
  operator double() const;
  /** Arithmetic operators (methods) */
  //@{
  Rational &operator+=(Rational const &);
  Rational &operator-=(Rational const &);
  Rational &operator/=(Rational const &);
  Rational &operator*=(Rational const &);
  Rational  operator-() const;
  Rational
  operator+() const
  {
    return *this;
  }
  Rational &operator++();
  Rational  operator++(int);
  Rational &operator--();
  Rational  operator--(int);
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
  int
  operator[](unsigned int i) const
  {
    return i == 0u ? M_n : M_d;
  }
  /** Arithmetic operators (friend classes) */
  //@{
  friend Rational operator+(Rational const &, Rational const &);
  friend Rational operator-(Rational const &, Rational const &);
  friend Rational operator*(Rational const &, Rational const &);
  friend Rational operator/(Rational const &, Rational const &);
  //@}

#if __cplusplus >= 202002L
/*!
 * @brief The spaceship operator
 * If we use c++20 we can define all 4 comparison operators in one go using spaceship.
 * I will then use the spaceship operator for integers (provided by the standard lib)
 * for the comparison.
 * Operator is friend to access private data directly and I use automatic return since
 * I will just use the ordering type returned by the spacship operator on integers (in fact, is a
 * std::strong_ordering)
 * 
 * I cannot default it becouse I need to implement it. It can be implemented also as a member function
 * @code {.cpp}
 * auto operator <=> (Rational const & r) const
 * {
 *  return M_n*r.M_d <=> M_n*r.M_d;
 * }
 * @endcode
 * but here I preferred to implement it as friend free function.
 * @param a the left operand
 * @param b the right operand
 * @return the ordering type
 */
friend auto operator <=> (Rational const &, Rational const & b);
/*!
 * @brief Equivalence operator
 * Since we store the rational always normalized I can use the default:
 * two Rationals are equivalent if both numerator and denominator are equal.
 */
friend bool operator ==(Rational const &, Rational const &)=default;
#else
  //! @brief Comparison operator
  //!
  //! In a pre c++20 code (but it is still perfectly valid!!) the best way to define
  //! consistent relational operators is to define the < operator and then all the others
  //! consistently. Here < is friend to allow accessing private data.
  //! @param l th eleft operand
  //! @param r the right operand
  //! @return true if l<r, false otherwise
  friend bool     operator<(Rational const & l, Rational const & r);
#endif
  //! Streaming operator to output rationals in a nice way.
  friend std::ostream &operator<<(std::ostream &, Rational const &);
  //! Streaming operator to input rationals in a nice way.
  friend std::istream &operator>>(std::istream &, Rational &);

private:
  int M_n, M_d;
  //! Helper function to normalize the internal rational.
  void M_normalize();
};
#if __cplusplus >= 202002L
inline auto operator <=> (Rational const & a, Rational const & b)
    {
  return a.M_n*b.M_d <=> b.M_n*a.M_d;
    }
#else
//* Logical operator definition (inlined for efficiency)
//! The less-than operator
inline bool
operator<(Rational const &l, Rational const &r)
{
  return (l.M_n * r.M_d) < (r.M_n * l.M_d);
}

//! Defined in terms of <
/*!
  @note For the Rational this is not necessary since the equality operator
  can easily be trasformed in equality between integers. It is only to stress
  that this is the general rule for equality operators.
*/
inline bool
operator==(Rational const &l, Rational const &r)
{
  return !(l < r) && !(r < l);
}
//! Defined in term of < and ==
inline bool
operator<=(Rational const &l, Rational const &r)
{
  return (l < r) || (l == r);
}
//! defined in terms of <=
inline bool
operator>(Rational const &l, Rational const &r)
{
  return !(l <= r);
}
//! defined in terms of <
inline bool
operator>=(Rational const &l, Rational const &r)
{
  return !(l < r);
}
#endif
/*!
 * @brief A hash functor for Rational objects
 *
 * It relies on the hash_compare utility in Utility/hashCompare.hpp
 *
 * @note If I use it to specialize std::hash<T> with T=Rational, I will not need to
 * pass it as template parameter of a unordered container
 *
 */
struct RationalHash
{
  /*!
   * @brief the call operator implementing the hash function
   * @param r The Rational object
   * @return The corresponding hash key
   */
  std::size_t operator()(const Rational & r)const
  {
    std::size_t seed{0u};
    apsc::hash_combine(seed, r.numerator(), r.denominator());
    return seed;
  }
};
} // namespace apsc
