/*
 * SortAndPermute.hpp
 *
 *  Created on: Mar 20, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_
#define EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_
#include <algorithm>  // for sort
#include <functional> // for less<T>
#include <utility>
#include <tuple>
#include <vector>
#include <numeric>
namespace apsc
{
  /*!
   * @struct SortAndPermuteTraits
   * @brief A trait that defines common types for the utilities in this file
   */
  struct
  SortAndPermuteTraits
  {
    using PermutationType=std::vector<std::size_t>;
  };
  /*!
   * @fn  sortAndPermute(Container&, CompOp=CompOp())
   * @brief Declaration of helper function
   *
   * This is a helper function to simplify the use of the utility SortAndPermute.
   *
   * @pre Container must be a stl compliant  random access container (a vector or array)
   * @tparam Container The type of the container to sort
   * @tparam CompOp The comparison operator for sorting
   * @param v The container to sort
   * @param comparison functor
   * @return A vector containing the permutation
   */

  template<typename Container,typename CompOp=std::less<typename Container::value_type>>
  auto sortAndPermute(Container &, CompOp comparison=CompOp());
/*!
 * @class SortAndPermute
 * @brief Allows to sort a sequential direct access container and extract the permutation
 *
 * You can then sort a container and apply the same permutation to other containers.
 *
 * @tparam Container A random access sequential container obeying Standard layout
 * @tparam CompOper A comparison operator on container elements
 */
template <class Container,
typename CompOper = std::less<typename Container::value_type>
>
class SortAndPermute
{
public:
  using value_type = typename Container::value_type;
  using PermutationType=SortAndPermuteTraits::PermutationType;

  /*!
   * @fn  SortAndPermute(CONT&&, const CompOper&=CompOper())
   * @brief Constructor
   *
   * The constructor allows to set the container to operate on and, if different than
   * "less than", a comparison operator.
   * @tparam CONT Container type, must be convertible to Container
   * @param v The container
   * @param comp The comparison operator
   */
  template<typename CONT>
  SortAndPermute(CONT&& v,const CompOper & comp=CompOper()): data_{std::forward<CONT>(v)},comparison{comp}
  {
    permutation_.resize(data_.size());
    // we initialize the identity permutation
    std::iota(permutation_.begin(),permutation_.end(),std::size_t(0));
  }

  /*!
   * @fn const Container data&()
   * @brief Returns the current container
   *
   * @return The container (sorted if operator() has been operated upon)
   */
  Container const & data(){return data_;}

  /*!
     * @fn const Container permutation&()
   * @brief Theturns the current permutation vector
   *
   * @return The permutation
   */
  PermutationType const & permutation(){return permutation_;}

  /*!
   * @fn void operator ()()
   * @brief It performs the actual permutation.
   *
   */
  void operator()();

  /*!
   * @fn void clear()
   * @brief Clers the stored data
   * Only if you need to save memory and if Container is a std::vector.
   */
    void clear()
  {
      data_.clear();
      data_.shrink_to_fit();
  }


  friend auto sortAndPermute<>(Container &, CompOper);
private:
  Container data_;
  CompOper comparison;
  PermutationType permutation_;
  /*!
   * Internal structure needed to keep track of the old position in the
   * container
   */
  struct element
  {
    value_type  value;
    std::size_t pos;
  };
};




template <class Container, typename CompOper>
void
SortAndPermute<Container,CompOper>::operator()()
{
  std::vector<element> elements;
  auto                 n = data_.size();
  elements.reserve(n);
  for(std::size_t i = 0; i < n; ++i)
    {
      elements.push_back({data_[i], i});
    }
  auto compOp = [this](element const &x, element const &y) {
    return comparison(x.value, y.value);
  };
  // Sort the container
  std::sort(elements.begin(), elements.end(), compOp);

  std::size_t index{0};
  for(auto const & [v,i]: elements)
    {
      data_[index]=v;
      permutation_[index++]=i;
    }
}

/*!
  * @fn Cont2 applyPermutation(const Cont2&, permutation const &p)
  * @brief Applies the permutation to another container
  *
  * @pre The container size should be equal to the saze of the sorted container
  * @tparam Cont2 The container type
  * @param cont2 The container
  * @return The container with the elements permuted
  */
 template< class Cont2>
 Cont2 applyPermutation(Cont2 const & cont2, SortAndPermuteTraits::PermutationType const & p)
 {
   Cont2 result;
   result.reserve(cont2.size());
   for (std::size_t i=0;i<cont2.size();++i)
     {
       result.emplace_back(cont2[p[i]]);
     }
   return result;
 }

 /*!
   * @fn void applyPermutationInPlace(Cont2&, permutation const& p)
   * @brief Applies permutation in place to another container
   *
   * @pre The container size should be equal to the saze of the sorted container
   * @tparam Cont2 The container type
   * @param cont2 A reference to the container that will be permuted.
   */
  template< class Cont2>
    void applyPermutationInPlace(Cont2 & cont2,SortAndPermuteTraits::PermutationType const & p)
  {
    // Thanks to Raymond Chen of Microsoft for the algorithm!
    auto indices=p;
    for (std::size_t i = 0; i < indices.size(); ++i)
      {
      auto current = i;
      while (i != indices[current])
        {
          auto next = indices[current];
          std::swap(cont2[current], cont2[next]);
          indices[current] = current;
          current = next;
      }
      indices[current] = current;
     }
  }




template<typename Container,typename CompOp>
auto sortAndPermute(Container & v, CompOp comparison)
{
  SortAndPermute<Container,CompOp> sortAndPermute{v,comparison};
  sortAndPermute();
  v=std::move(sortAndPermute.data_);
  return sortAndPermute.permutation_;
}






} // namespace apsc

#endif /* EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_ */
