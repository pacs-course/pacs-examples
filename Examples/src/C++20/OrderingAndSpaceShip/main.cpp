/*
 * main.cpp
 *
 *  Created on: Jan 15, 2022
 *      Author: forma
 */
#include <algorithm>
#include <compare>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_set>

#include <vector>
// C++20 introduces the concept of weak/partial/strong ordering.
// and the spaceship operator that allows to define < > and == in one go
/*!
 * A class to define a 2-dimensional Point
 */
struct Point
{
  double x = 0.0;
  double y = 0.0;
  //! Spaceship operator defaulted
  //!
  //! By doing so you have all 6 relational operator with default semantic.
  //! The default ordering relation is the lexicographic one
  //! The return ordering type is the weakeast ordering among members (in this case it will be a std::strong_ordering!)
  //! But you may just use auto and ignore the details, since the compiler will do the right thing and you have the
  //! expected conversion to bool
  friend auto operator<=>(const Point &, const Point &)= default;
};

/*!
 * A simple class for rationals, where I do not normalize the Rational
 *
 */
struct Rational
{
  int den = 1;
  int num = 0;
  /*! @brief spaceshift for this simple representation of rational numbers

   If the rational is not normalised I have a weak ordering
   since two equivalent elements are not "equal": 1/2 and 2/4 are
   equivalent but if I have a function that returns the numerator
   f(1/2) != f(2/4)
   Thus, I  implement a spaceship returning a weak ordering
    */
  friend std::weak_ordering
  operator<=>(Rational const & r1, Rational const &r2);
 /*!
  * Having defined <=> does not give == automatically
  * what I have is instead the functor std::is_eq()
  * I need to define == if I want it. I decide to be consistent and
  * say that ==  returns is_eq().
  * @param r1 A rational
  * @param r2 A rational
  * @return true if equivalent according to what defined in <=>
  */
  friend bool operator ==(Rational const & r1, Rational const & r2);
};

inline std::weak_ordering
operator<=>(Rational const & r1, Rational const &r2)
{
  using po = std::weak_ordering;
  auto a = r1.num * r2.den;
  auto b = r1.den * r2.num;
  if(a < b)
    return po::less;
  if(b < a)
    return po::greater;
  else
    return po::equivalent;
}

inline bool operator== (Rational const & r1, Rational const & r2)
    {
    return std::is_eq(r1<=>r2);
    }
/*!
 * Another simple class for rationals, where I do not normalize the Rational
 *
 */
struct Rational2
{
  int den = 1;
  int num = 0;
  /*! @brief spaceshift for this simple representation of rational numbers

   If the rational is not normalised I have a weak ordering
   since two equivalent elements are not "equal": 1/2 and 2/4 are
   equivalent but if I have a function that returns the numerator
   f(1/2) != f(2/4)
   Thus, I  implement a spaceship returning a weak ordering
    */
  friend std::weak_ordering
  operator<=>(Rational2 const & r1, Rational2 const &r2);
 /*!
  * Having defined <=> does not give == automatically
  * I decide to default ==. It returns true if both numerators and denominators are equal
  * @param r1 A rational
  * @param r2 A rational
  * @return true if den and num equal
  */
  friend bool operator ==(Rational2 const & r1, Rational2 const & r2)=default;
};

inline std::weak_ordering
operator<=>(Rational2 const & r1, Rational2 const &r2)
{
  using po = std::weak_ordering;
  auto a = r1.num * r2.den;
  auto b = r1.den * r2.num;
  if(a < b)
    return po::less;
  if(b < a)
    return po::greater;
  else
    return po::equivalent;
}



struct Poset
{
  /*!
   * @brief spaceship operator for partial ordering
   *
   * Here I say that two Posets are comparable only if the
   * data has the same length. Otherwise I follow lexicographic ordering.
   * By doing so I create a partial ordering.
   */
  std::vector<int> data;
  std::partial_ordering
  operator<=>(Poset const &r2)
  {
    using po = std::partial_ordering;
    if(std::ranges::equal(data, r2.data))
      return po::equivalent;
    if(r2.data.size() != data.size())
      return po::unordered;
    if(std::ranges::lexicographical_compare(data, r2.data))
      return po::less;
    return po::greater;
  }
};


  int
  main()
  {
    Point a{4., 5.};
    Point b{5., 6.};
    std::cout<<"Two different points a=(4,5) and b=(5,6)\n";
    std::cout << std::boolalpha << "(a < b)? "<< (a < b) << ", (a == b)? " << (a == b) << std::endl;

    {
    Rational r1{1, 2};
    Rational r2{2, 4};
    std::cout<<"Two Rationals r1=1/2 and r2=2/4\n";
    std::cout << std::boolalpha << "r1 eqv r2? "<<std::is_eq(r1 <=> r2) << ", r1==r2? "<< (r1==r2) <<std::endl;
    std::cout << std::boolalpha << "r1 < r2? "<< (r1<r2) <<std::endl;
   }
    {
     Rational2 r1{1, 2};
     Rational2 r2{2, 4};
     std::cout<<"Two Rational2s r1=1/2 and r2=2/4\n";
     std::cout << std::boolalpha << "r1 eqv r2? "<<std::is_eq(r1 <=> r2) << ", r1==r2? "<< (r1==r2) <<std::endl;
     std::cout << std::boolalpha << "r1 < r2? "<< (r1<r2) <<std::endl;
    }





  Poset ap{{1,2,3,4}};
  Poset bp{{1,2}};
  std::cout<<"This two posets, ap and bp, are not comparable.\n";
  std::cout<<std::boolalpha<<"ap<bp? "<<(ap<bp)<<", ap>=bp? "<<(ap>=bp)<<", ap eq bp? "<<std::is_eq(ap<=>bp)<<std::endl;




}
