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
struct Point
{
  double x = 0.0;
  double y = 0.0;
  //! Spaceship operator defaulted
  //!
  //! By doing so you have all 6 relational operator with default semantic.
  //! The default ordering relation is the lexicographic one
  //! The ordering type is the weakeast ordering among members.
  //! But you may just use auto and ignore the details
  friend auto operator<=>(const Point &, const Point &)= default;
};

struct Rational
{
  int den = 1;
  int num = 0;
  //! @brief spaceshift fo this simple representation of rational numbers
  //! If the rational is not normalised I have a weak ordering
  //! since two equivalent elements are not "equal": 1/2 and 2/4 are
  //! equivalent but if I have a function that returns the numerator
  //! f(1/2) != f(2/4)
  //!  I implement a spaceship returning a weak ordering
  std::weak_ordering
  operator<=>(Rational const &r2) const
  {
    using po = std::weak_ordering;
    auto a = num * r2.den;
    auto b = den * r2.num;
    if(a < b)
      return po::less;
    if(b < a)
      return po::greater;
    else
      return po::equivalent;
  }
  //! I want == to return true on equivalence
  //!
  //! In this case a true a==b  does not mean interchangeable object:
  //! two equivalent rationals
  bool operator ==(Rational const & r2)const
    {
    return std::is_eq(*this<=>r2);
    }

};

struct OddEquivalence
{
  int x = 0;
  /*!
   * @brief another example of weak ordering
   *
   * I create the equivanence class of even/odd numbers by defining the
   * appropriate operator
   */

  std::weak_ordering
  operator<=>(OddEquivalence const &r2) const
  {
    using po = std::weak_ordering;
    auto a = x % 2;
    auto b = r2.x % 2;
    if(a < b)
      return po::less;
    if(b < a)
      return po::greater;
    else
      return po::equivalent;
  }
  //! I want operator == that tests equality and I am using the default one
  friend bool operator ==(OddEquivalence const &, OddEquivalence const &)=default;
};

struct OddEquivalence2
{
  int x = 0;
  /*!
   * @brief another example of weak ordering
   *
   * I create the equivalence class of even/odd numbers by defining the
   * appropriate operator.
   */

  std::weak_ordering
  operator<=>(OddEquivalence2 const &r2) const
  {
    using po = std::weak_ordering;
    auto a = x % 2;
    auto b = r2.x % 2;
    if(a < b)
      return po::less;
    if(b < a)
      return po::greater;
    else
      return po::equivalent;
  }
  //! I want operator == to test equivalence
  friend bool operator ==(OddEquivalence2 const &, OddEquivalence2 const &);
};
bool operator ==(OddEquivalence2 const & a, OddEquivalence2 const & b)
    {
  return std::is_eq(a<=>b);
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
  std::cout << std::boolalpha << (a < b) << " " << (a == b) << std::endl;

  Rational r1{10, 20};
  Rational r2{20, 40};
  std::cout << std::boolalpha << std::is_eq(r1 <=> r2) << " "<< (r1==r2) <<std::endl;


  {
  OddEquivalence aa{3};
  OddEquivalence bb{5};
  // Equivalent but not equal! Here is_eq and == do not return the same!
  std::cout << std::boolalpha << std::is_eq(aa<=>bb) << " " << (aa == bb) << std::endl;
  std::set<OddEquivalence> ms;
  for (int i=0;i<20;++i) ms.emplace(i);
  std::cout<<"Size of set of OddEquivalence objects "<<ms.size()<<std::endl;
  // You may not that size is 2 since set uses equivalence
  }

  {
    OddEquivalence2 aa{3};
    OddEquivalence2 bb{5};
    // Here == do return equivalence!
    std::cout << std::boolalpha << std::is_eq(aa<=>bb) << " " << (aa == bb) << std::endl;
    std::set<OddEquivalence2> ms;
    for (int i=0;i<20;++i) ms.emplace(i);
    std::cout<<"Size of set of OddEquivalence2 objects "<<ms.size()<<std::endl;
    // You may not that size is 2 since set uses equivalence
    }




  Poset ap{{1,2,3,4}};
  Poset bp{{1,2}};
  std::cout<<"This two posets are not comparable.\n";
  std::cout<<std::boolalpha<<(ap<bp)<<(ap>bp)<<std::is_eq(ap<=>bp)<<std::endl;




}
