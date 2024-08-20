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
#include <numeric>
#include <ranges> // for ranges concepts
#include <tuple>
#include <utility>
#include <vector>
namespace apsc
{
/*
 * @fn  sortAndPermute(Container&, CompOp=CompOp())
 * @brief Sorts a sequential container and returns the permutation vector
 *   *
 * @pre Container must be a stl compliant  random access container (a vector or
 * array)
 * @tparam Container The type of the container to sort. Satisfies the
 * random_acces_range concept
 * @tparam CompOp The comparison operator for sorting
 * @param v The container to sort
 * @param comparison functor
 * @return A vector of integers containing the permutation
 */
template <std::ranges::random_access_range Container,
          typename CompOp = std::less<typename Container::value_type>>
  requires std::totally_ordered<typename Container::value_type>
std::vector<std::size_t> sortAndPermute(Container &,
                                        CompOp const &comparison = CompOp());

/*!
 * @fn Cont2 applyPermutation(const Cont2&, permutation const &p)
 * @brief Applies the permutation to another container
 *
 * @pre The container size should be equal to the saze of the sorted container
 * @tparam Cont2 The container type
 * @param cont2 The container
 * @param p The permutation vector
 * @return The container with the elements permuted
 */
template <class Cont2>
Cont2 applyPermutation(Cont2 const &cont2, std::vector<std::size_t> const &p);

/*!
 * @fn void applyPermutationInPlace(Cont2&, permutation const& p)
 * @brief Applies permutation in place to another container
 *
 * @pre The container size should be equal to the saze of the sorted container
 * @tparam Cont2 The container type
 * @param cont2 A reference to the container that will be permuted in place.
 * @param p The permutation vector
 */
template <class Cont2>
void applyPermutationInPlace(Cont2 &cont2, std::vector<std::size_t> const &p);

/*!
 * The permutation vector retuned by sortAndPermute contains in position n the
 * index of the position in the original (unsorted) container of the n-th
 * element of the sorted container:
 *
 *     auto original=v;
 *     auto p =sortAndPermute(v);
 *     v[i] = original[p[i]];
 *
 * Sometimes you need the inverse:
 *
 *    v[ip[i]] = original[i];
 *
 *
 * @param p The permutation vector
 * @return The inverse permutation ip
 */
inline std::vector<std::size_t>
invertPermutation(std::vector<std::size_t> const &p)
{
  std::vector<std::size_t> ip(p.size());
  for(std::size_t i = 0u; i != p.size(); ++i)
    ip[p[i]] = i;
  return ip;
}

/*!
 * @class SortAndPermute
 * @brief Allows to sort a sequential direct access container and extract the
 * permutation
 *
 * You can then sort a container and apply the same permutation to other
 * containers.
 *
 * @tparam Container A random access sequential container obeying Standard
 * layout
 * @tparam CompOper A comparison operator on container elements
 */
template <std::ranges::random_access_range Container,
          typename CompOper = std::less<typename Container::value_type>>
class SortAndPermute
{
public:
  using value_type = typename Container::value_type;
  using PermutationType = std::vector<size_t>;

  /*!
   * @fn  SortAndPermute(CONT&&, const CompOper&=CompOper())
   * @brief Constructor
   *
   * The constructor allows to set the container to operate on and, if different
   * than "less than", a comparison operator.
   * @tparam CONT Container type, must be convertible to Container
   * @param v The container
   * @param comp The comparison operator
   */
  template <std::ranges::random_access_range CONT>
  SortAndPermute(CONT &&v, const CompOper &comp = CompOper())
    : data_{std::forward<CONT>(v)}, comparison{comp}
  {
    permutation_.resize(data_.size());
    // we initialize the identity permutation
    std::iota(permutation_.begin(), permutation_.end(), std::size_t(0));
  }

  /*!
   * @fn const Container data&()
   * @brief Returns the current container
   *
   * @return The container (sorted if operator() has been operated upon)
   */
  Container const &
  data()
  {
    return data_;
  }

  /*!
   * @fn const Container permutation&()
   * @brief Theturns the current permutation vector
   *
   * @return The permutation
   */
  PermutationType const &
  permutation()
  {
    return permutation_;
  }

  /*!
   * @fn void operator ()()
   * @brief It performs the sorting and  permutation.
   *
   */
  void operator()();

  /*!
   * @fn void clear()
   * @brief Clers the stored data
   * Only if you need to save memory and if Container is a std::vector.
   */
  void
  clear()
  {
    data_.clear();
    data_.shrink_to_fit();
  }

  friend PermutationType sortAndPermute<>(Container &, CompOper const &);

private:
  Container       data_;
  CompOper        comparison;
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

//   ********************    IMPLEMENTATIONS   ********************

template <std::ranges::random_access_range Container, typename CompOper>
void
SortAndPermute<Container, CompOper>::operator()()
{
  std::vector<element> elements;
  auto                 n = data_.size();
  elements.reserve(n);
  for(std::size_t i = 0u; i < n; ++i)
    {
      elements.emplace_back(data_[i], i);
    }

  // I use here the new constrained algorithms to sort the container
  // I use the projection to sort the elements by value
  std::ranges::sort(elements, comparison, &element::value);

  /* Before c+20
    auto compOp = [this](element const &x, element const &y) {
      return comparison(x.value, y.
     std::size_t index{0};
    for(auto const & [v,i]: elements)
      {
        data_[index]=v;
        permutation_[index++]=i;
      }value);
    };
    // Sort the container
    std::sort(elements.begin(), elements.end(), compOp);
  */
  // I fill back the permutation vector
  for(std::size_t i = 0; i != elements.size(); ++i)
    {
      permutation_[i] = elements[i].pos;
      data_[i] = elements[i].value;
    }
}

template <std::ranges::random_access_range Cont2>
Cont2
applyPermutation(Cont2 const &cont2, std::vector<std::size_t> const &p)
{
  Cont2 result;
  result.reserve(cont2.size());
  for(std::size_t i = 0; i < cont2.size(); ++i)
    {
      result.emplace_back(cont2[p[i]]);
    }
  return result;
}

template <std::ranges::random_access_range Cont2>
void
applyPermutationInPlace(Cont2 &cont2, std::vector<std::size_t> const &p)
{
  // Thanks to Raymond Chen of Microsoft for the algorithm!
  auto indices = p;
  for(std::size_t i = 0; i < indices.size(); ++i)
    {
      auto current = i;
      while(i != indices[current])
        {
          auto next = indices[current];
          std::swap(cont2[current], cont2[next]);
          indices[current] = current;
          current = next;
        }
      indices[current] = current;
    }
}

template <std::ranges::random_access_range Container, typename CompOp>
  requires std::totally_ordered<typename Container::value_type>
std::vector<std::size_t>
sortAndPermute(Container &v, CompOp const &comparison)
{
  SortAndPermute<Container, CompOp> sortAndPermute{v, comparison};
  sortAndPermute();
  v = std::move(sortAndPermute.data_);
  return sortAndPermute.permutation_;
}

} // namespace apsc

#endif /* EXAMPLES_SRC_STL_SORTANDPERMUTE_SORTANDPERMUTE_HPP_ */
