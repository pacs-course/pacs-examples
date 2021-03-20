/*
 * SortAndPermute.hpp
 *
 *  Created on: Mar 20, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_
#define EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_
#include <algorithm> // for sort
#include <functional> // for less<T>
namespace apsc
{
  /*!
   * This class is a handle to a sequential random access container (vector or array) of the
   * standard library. It operates directly on the container passed in the constructor and returns
   * with the call operator the sorted container (as side effect) and the permutation
   * old position -> new position after sorting.
   *
   * permutation[i] is the new position in the container of the i-th element before sorting
   *
   * @tparam Container The container
   */
  template <class Container>
  class
  SortAndPermute
  {
  public:
    using value_type = typename Container::value_type;
    //! The constructor takes the container by reference
    SortAndPermute(Container & v):data{v}{}
    /*!
     * The call operator performs the sorting using the comparision operator
     * passed as argument, and defaulted to less-than, and returns the permutation.
     * @tparam CompOper The type of comparison operator on the container elements (defaulted to less<>)
     * @param compOper  The comparison operator on the container elements. Defaulted to a less<> object
     * @return The permutation old position ->new position
     */
    template<typename CompOper=std::less<value_type> >
    std::vector<std::size_t> operator()(CompOper compOper=CompOper{});
  private:
    Container & data;
    /*!
     * Internal structure needed to keep track of the old position in the container
     */
    struct
    element
    {
      value_type value;
      std::size_t pos;
    };
  };

  template <class Container>
  template<typename CompOper>
  std::vector<std::size_t> SortAndPermute<Container>::operator()(CompOper compOper)
  {
    std::vector<element> elements;
    auto n = data.size();
    elements.reserve(n);
    for (std::size_t i=0; i<n;++i)
      {
        elements.push_back({data[i],i});
      }
    auto compOp=[&compOper](element const & x, element const & y)
                {
      return compOper(x.value,y.value);
                };
    // Sort the container
    std::sort(elements.begin(),elements.end(),compOp);
    // Get the permutation
    std::vector<std::size_t> permutation;
    permutation.reserve(n);
    for (std::size_t i=0; i<n;++i)
      {
        data[i]=elements[i].value;
        permutation.push_back(elements[i].pos);
      }
    return permutation;
  }
}




#endif /* EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_ */
