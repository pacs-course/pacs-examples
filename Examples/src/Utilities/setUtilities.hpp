#ifndef HH__SETUTILITIES_HH
#define HH__SETUTILITIES_HH
#include <algorithm>
#include <iterator>

/*
  Some utilities to simplify the use of set operations
  I have used the same name of the utilities in the Standard Library
  (but a different namespace)
*/
namespace Utility
{
  /*!
    Difference of two ordered containers.
    @tparam OrderedContainer An ordered container (set or map)
    @param a the first container
    @param b the second container
    @return \f$ a\setminus b \f$
  */
  template<class OrderedContainer>
  auto set_difference(OrderedContainer const & a,
                      OrderedContainer const & b) -> OrderedContainer
  {
    OrderedContainer c;
    using Compare = typename OrderedContainer::key_compare;
    std::set_difference(a.begin(),a.end(),
                        b.begin(),b.end(),
                        std::inserter(c,c.begin()),
                        Compare{});
    return c;
  }
  /*!
    Union of two ordered containers.
    @tparam OrderedContainer An ordered container (set or map)
    @param a the first container
    @param b the second container
    @return \f$ a\cup b \f$
  */

  template<class OrderedContainer>
  auto set_union(OrderedContainer const & a,
                        OrderedContainer const & b) -> OrderedContainer
  {
    OrderedContainer c;
    using Compare = typename OrderedContainer::key_compare;
    std::set_union(a.begin(),a.end(),
                   b.begin(),b.end(),
                   std::inserter(c,c.begin()),
                   Compare{});
    return c;
  }
  /*!
    Intersection of two ordered containers.
    @tparam OrderedContainer An ordered container (set or map)
    @param a the first container
    @param b the second container
    @return \f$ a\cap b \f$
  */

  template<class OrderedContainer>
  auto set_intersection(OrderedContainer const & a,
                          OrderedContainer const & b) -> OrderedContainer
  {
    OrderedContainer c;
    using Compare = typename OrderedContainer::key_compare;
    std::set_intersection(a.begin(),a.end(),
                          b.begin(),b.end(),
                          std::inserter(c,c.begin()),
                          Compare{});
    return c;
  }
}
#endif
