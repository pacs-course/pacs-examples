/*
 * joinVectors.hpp
 *
 *  Created on: Jan 21, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_
#define EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <vector>
namespace apsc
{
/*
 * Forward declarations
 */
template <class... Types> class JoinVectorConstIterator;
template <class... Types> class JoinVectorIterator;

/*!
 * @brief Zips several std::vector objects into a single view.
 *
 * The class stores pointers to a pack of vectors and exposes element access
 * by position. Accessing position i returns a tuple containing the i-th
 * element of each vector.
 *
 * @tparam Types Value types stored in the vectors to be joined
 * @note All joined vectors must have the same size.
 * @note The object does not own the vectors. They must outlive the view.
 */
template <class... Types> class JoinVectors
{
public:
  //! The number of vectors joined
  static constexpr std::size_t numVectors = sizeof...(Types);
  static_assert(numVectors > 0u,
                "JoinVectors requires at least one vector to join");
  //! The tuple type  containing the pointers to the joined vectors
  using ContainingTuple = std::tuple<std::vector<Types> *...>;
  //! The type of a tuple containing the value types stored in the joined
  //! vectors
  using Values = std::tuple<Types...>;
  JoinVectors() = default;
  /*!
   * @brief Constructs the joined view from a pack of vectors.
   * @param args Vectors to join
   * @throws std::invalid_argument if the vectors do not all have the same size
   */
  JoinVectors(std::vector<Types> &...args) { set_Vectors(args...); }

  /*! @brief Binds the view to a new pack of vectors.
   * @param args Vectors to join
   * @throws std::invalid_argument if the vectors do not all have the same size
   */
  void
  set_Vectors(std::vector<Types> &...args)
  {
    ensure_same_size(args...);
    data = {&args...};
  }

  /*!
   * @brief Returns a tuple of values at position i.
   * @param i The index of the values to be extracted
   * @return A tuple containing copies of the i-th elements of all joined
   * vectors
   * @pre The view must be bound to vectors.
   * @note Bounds are not checked.
   */
  Values
  operator[](std::size_t i) const
  {
    ensure_initialized();
    return std::apply([i](auto *...x) { return Values{(*x)[i]...}; }, data);
  }

  /*!
   * @brief Returns a tuple of references at position i.
   * @param i The index of the values to be extracted
   * @return A tuple of references to the i-th elements of all joined vectors
   * @pre The view must be bound to vectors.
   * @note Bounds are not checked.
   */
  decltype(auto)
  operator[](std::size_t i)
  {
    ensure_initialized();
    return std::apply([i](auto *...x) { return std::tie((*x)[i]...); }, data);
  }
  //! The size of the vectors stored in the joined vector
  std::size_t
  size() const
  {
    ensure_initialized();
    return std::get<0>(data)->size();
  }
  //! @return a const input iterator
  auto
  begin() const
  {
    ensure_initialized();
    return apsc::JoinVectorConstIterator{*this}.createBegin();
  }
  //! @return a non-const input iterator
  auto
  begin()
  {
    ensure_initialized();
    return apsc::JoinVectorIterator{*this}.createBegin();
  }
  //! @return a const input iterator representing the end of the joined view
  auto
  end() const
  {
    ensure_initialized();
    return apsc::JoinVectorConstIterator{*this}.createEnd();
  }
  //! @return a non-const input iterator representing the end of the joined view
  auto
  end()
  {
    ensure_initialized();
    return apsc::JoinVectorIterator{*this}.createEnd();
  }

private:
  void
  ensure_initialized() const
  {
    if(std::get<0>(data) == nullptr)
      {
        throw std::logic_error("JoinVectors is not bound to any vectors");
      }
  }

  static void
  ensure_same_size(std::vector<Types> const &...args)
  {
    auto const first_size = std::get<0>(std::forward_as_tuple(args...)).size();
    if(!((args.size() == first_size) && ...))
      {
        throw std::invalid_argument(
          "JoinVectors requires all vectors to have the same size");
      }
  }

  ContainingTuple data{};
};

/*!
 * @brief Convenience helper creating a JoinVectors view.
 * @tparam Types Value types stored in the vectors to join
 * @param args Vectors to join
 * @return A JoinVectors object bound to args
 */
template <class... Types>
auto
join_vectors(std::vector<Types> &...args)
{
  return JoinVectors<Types...>{args...};
}

/*!
 * @brief Read-only input iterator for JoinVectors.
 *
 * The iterator traverses a JoinVectors view and yields tuples of values.
 * It is intended for single-pass input-style iteration.
 * @tparam Types Value types stored in the joined vectors
 */
template <class... Types> class JoinVectorConstIterator
{
public:
  /*! @name Iterator associated types
   * These aliases describe the iterator category and dereference result.
   */
  using difference_type = std::ptrdiff_t;
  using value_type = typename JoinVectors<Types...>::Values;
  using pointer = void;
  using reference = value_type;
  using const_reference = value_type;
  using iterator_category = std::input_iterator_tag;
  using iterator_concept = std::input_iterator_tag;

  //! Constructor. It takes the JoinVectors object to iterate over.
  JoinVectorConstIterator(JoinVectors<Types...> const &myV) : myVector{myV} {}
  //! @return The tuple of values at the current position.
  reference
  operator*() const
  {
    return myVector[counter];
  }
  //! @return Pointer to the underlying joined view (allows it->size() etc.)
  JoinVectors<Types...> const *
  operator->() const noexcept
  {
    return &myVector;
  }
  /*!
   * @brief Prefix increment.
   * @note no bounds check!
   * @return The advanced iterator
   */
  auto &
  operator++()
  {
    ++counter;
    return *this;
  }
  /*!
   * @brief Postfix increment.
   * @return The iterator value before increment
   */
  auto
  operator++(int)
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  /*!
   * @brief Advance in place by a positive integral value.
   * @param i The value
   * @return The advanced iterator
   * @note No bounds check!
   */
  auto &
  operator+=(std::size_t i)
  {
    counter += i;
    return *this;
  }
  /*!
   * @brief Returns an iterator advanced by a positive integral value.
   * @param i The increment
   * @return A copy of the iterator advanced by i positions
   * @note No bounds check!
   */
  auto
  operator+(std::size_t i) const
  {
    auto tmp = *this;
    tmp += i;
    return tmp;
  }
  /*!
   * @brief Decrement in place by a positive integral value.
   * @param i The value
   * @return The decremented iterator
   * @note No bounds check! If i>counter result is undetermined (probably
   * segmentation fault!)
   */
  auto &
  operator-=(std::size_t i)
  {
    counter -= i;
    return *this;
  }
  /*!
   * @brief Returns a decremented copy of the iterator.
   * @param i The decrement
   * @return A copy of the iterator decremented by i positions
   * @note No bounds check! If i>counter the behaviour is undefined.
   */
  auto
  operator-(std::size_t i) const
  {
    auto tmp = *this;
    tmp -= i;
    return tmp;
  }
  /*!
   * @brief Creates the iterator that represents the end.
   * @return the end iterator
   */
  auto
  createEnd() const
  {
    JoinVectorConstIterator res(myVector);
    res.counter = myVector.size();
    return res;
  }
  //! Creates the iterator that represents begin
  auto
  createBegin() const
  {
    return JoinVectorConstIterator{myVector};
  }
  //! Gives read-only access to the underlying joined view.
  JoinVectors<Types...> const *
  theJoinVector() const
  {
    return &myVector;
  }
  //! Current position in the joined view.
  std::size_t counter = 0u;

private:
  JoinVectors<Types...> const &myVector;
};

template <class... Types>
bool
operator<(const JoinVectorConstIterator<Types...> &a,
          const JoinVectorConstIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return a.counter < b.counter;
}

template <class... Types>
bool
operator==(const JoinVectorConstIterator<Types...> &a,
           const JoinVectorConstIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return a.counter == b.counter;
}

template <class... Types>
bool
operator!=(const JoinVectorConstIterator<Types...> &a,
           const JoinVectorConstIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return !(a == b);
}

/*!
 * @brief Mutable input iterator for JoinVectors.
 *
 * Dereferencing returns a tuple of references to the current elements of the
 * joined vectors.
 * @tparam Types Value types stored in the joined vectors
 */
template <class... Types> class JoinVectorIterator
{
public:
  /*! @name Iterator associated types
   * These aliases describe the iterator category and dereference result.
   */
  using difference_type = std::ptrdiff_t;
  using value_type = std::tuple<Types &...>;
  using pointer = void;
  using reference = value_type;
  using const_reference = value_type;
  using iterator_category = std::input_iterator_tag;
  using iterator_concept = std::input_iterator_tag;

  //! Constructor. It takes the JoinVectors object to iterate over.
  JoinVectorIterator(JoinVectors<Types...> &myV) : myVector{myV} {}
  //! @return The tuple of references at the current position.
  reference
  operator*() const
  {
    return myVector[counter];
  }
  //! @return Pointer to the underlying joined view (allows it->size() etc.)
  JoinVectors<Types...> const *
  operator->() const noexcept
  {
    return &myVector;
  }
  /*!
   * @brief Prefix increment.
   * @note no bounds check!
   * @return The advanced iterator
   */
  auto &
  operator++()
  {
    ++counter;
    return *this;
  }
  /*!
   * @brief Postfix increment.
   * @return The iterator value before increment
   */
  auto
  operator++(int)
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  /*!
   * @brief Advance in place by a positive integral value.
   * @param i The value
   * @return The advanced iterator
   * @note No bounds check!
   */
  auto &
  operator+=(std::size_t i)
  {
    counter += i;
    return *this;
  }
  /*!
   * @brief Returns an iterator advanced by a positive integral value.
   * @param i The increment
   * @return A copy of the iterator advanced by i positions
   * @note No bounds check!
   */
  auto
  operator+(std::size_t i) const
  {
    auto tmp = *this;
    tmp += i;
    return tmp;
  }
  /*!
   * @brief Decrement in place by a positive integral value.
   * @param i The value
   * @return The decremented iterator
   * @note No bounds check! If i>counter result is undetermined (probably
   * segmentation fault!)
   */
  auto &
  operator-=(std::size_t i)
  {
    counter -= i;
    return *this;
  }
  /*!
   * @brief Returns a decremented copy of the iterator.
   * @param i The decrement
   * @return A copy of the iterator decremented by i positions
   * @note No bounds check! If i>counter the behaviour is undefined.
   */
  auto
  operator-(std::size_t i) const
  {
    auto tmp = *this;
    tmp -= i;
    return tmp;
  }
  /*!
   * @brief Creates the iterator that represents the end.
   * @return the end iterator
   */
  auto
  createEnd() const
  {
    JoinVectorIterator res(myVector);
    res.counter = myVector.size();
    return res;
  }
  //! Creates the iterator that represents begin
  auto
  createBegin() const
  {
    return JoinVectorIterator{myVector};
  }
  //! Gives read-only access to the underlying joined view.
  JoinVectors<Types...> const *
  theJoinVector() const
  {
    return &myVector;
  }
  //! Current position in the joined view.
  std::size_t counter = 0u;

private:
  JoinVectors<Types...> &myVector;
};

template <class... Types>
bool
operator<(const JoinVectorIterator<Types...> &a,
          const JoinVectorIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return a.counter < b.counter;
}

template <class... Types>
bool
operator==(const JoinVectorIterator<Types...> &a,
           const JoinVectorIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return a.counter == b.counter;
}

template <class... Types>
bool
operator!=(const JoinVectorIterator<Types...> &a,
           const JoinVectorIterator<Types...> &b)
{
  if(a.theJoinVector() != b.theJoinVector())
    throw std::runtime_error(
      "Cannot compare two iterators to different joined vectors");
  return !(a == b);
}

} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_JOINVECTORS_HPP_ */
