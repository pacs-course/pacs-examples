//
// Created by forma on 18/06/23.
//

#ifndef EXAMPLES_HEAPVIEW_HPP
#define EXAMPLES_HEAPVIEW_HPP
#include "heapViewTraits.hpp"
#include <concepts>
#include <functional>
#include <limits>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <utility>

/*!
 * @brief A heap view is a data structure that allows you to have
 * values stored in a and operate on them as a heap without moving them around
 * in memory.
 *
 * The heap view is a wrapper around a vector of data elements.  The heap view
 * maintains a heap of indices into the vector.  The heap view also maintains a
 * vector of indices into the heap.  The heap view is a view of the data
 * elements, so the data elements are not moved around in memory.  The heap
 * view is a heap of indices, so the indices are moved around in memory
 * not the data.  The heap view maintains a mapping from data
 * indices to indices into the heap.
 *
 * @note At the moment is is not implemented as a view since it has a copy of
 * the data vector and not a reference. A view would be better.
 * @tparam ElementType The type stored in the data vector. It should be
 * comparable with the comparison operator.
 * @tparam CompOp Comparison operator for the data elements.  It should be
 * proper comparison operator on DataElementType.
 * @tparam Traits The traits for the heap view.  It should provide the
 * definition of the internal types.
 */
namespace apsc
{
template <class DataElementType, class CompOp = std::less<DataElementType>,
          class Traits = heapViewTraits<DataElementType>>
class HeapView
{
public:
  using Index = Traits::Index;
  using DataVector = Traits::DataVector;
  using DataIndex = Traits::DataIndex;
  using ElementType = Traits::ElementType;
  using HeapIndex = Traits::HeapIndex;
  using HeapIter = Traits::HeapIter;

  HeapView() = default;
  /*!
   * @brief Construct a heap view from a vector of data elements.
   * @param data The vector of data elements.
   * @param comp The comparison operator for the data elements. Defaults to the
   * default constructed one.
   */
  template <class T>
  explicit HeapView(T &&data, CompOp comp = CompOp{})
    : data_(std::forward<T>(data)), comp_(comp)
  {
    if(data_.size() != 0)
      {
        heapIndex_.reserve(data_.size());
        heapIter_.reserve(data_.size());
        for(Index i = 0; i < data_.size(); ++i)
          {
            heapIndex_.push_back(i);
            heapIter_.push_back(i);
          }
        for(Index i = data_.size() / 2; i > 0; --i)
          {
            siftDown(i);
          }
        siftDown(0);
      }
  }
  HeapView(const HeapView &other) = default;
  HeapView(HeapView &&other) noexcept = default;
  HeapView &operator=(const HeapView &other) = default;
  HeapView &operator=(HeapView &&other) noexcept = default;
  /*!
   * @brief Construct a heap view from a vector of data elements.
   * @param data The data
   */
  template <class T = DataVector>
  void
  setData(T &&data)
  {
    data_ = std::forward<T>(data);
    heapIndex_.clear();
    heapIter_.clear();
    heapIndex_.reserve(data_.size());
    heapIter_.reserve(data_.size());
    for(Index i = 0; i < data_.size(); ++i)
      {
        heapIndex_.emplace_back(i);
        heapIter_.emplace_back(i);
      }
    if(data_.size() != 0)
      {
        // to avoid problems with subtraction of unsigned I treat index 0
        // specially
        for(Index i = data_.size() / 2; i > 0; --i)
          {
            siftDown(i);
          }
        siftDown(0);
      }
  }
  /*!
   * @brief Reserves capacity.
   */
  void
  reserve(std::size_t n)
  {
    data_.resize(n);
    heapIndex_.reserve(n);
    heapIter_.resize(n);
  }
  /*  @brief Returns the underlying data vector.
   *  @return The underlying data vector.
   *  @note The data vector is given as a const reference: you cannot
   *  change from the outside the content of the internal data vector.
   */
  auto const &
  data() const
  {
    return data_;
  }
  /*!
   * Adds a new element to the heap view.
   * @param e The element to add
   * @return The index in the heap where the element was added
   */
  template <class E>
  Index
  add(E &&e)
  {
    Index i = this->size();
    data_.emplace_back(std::forward<E>(e));
    heapIndex_.emplace_back(data_.size() - 1u);
    heapIter_.emplace_back(i);
    return siftUp(i);
  }
  /*!
   * @brief Remove an element from the heap view.
   * @param i The index of the element to remove.
   * @return The index in the heap where the element was removed.
   * @note The element is not removed from the data vector.
   */
  Index
  remove(DataIndex i)
  {
    auto where = heapIter_[i];
    if(not where.has_value())
      {
        throw std::invalid_argument(
          "HeapView: trying to remove an element not in the heap");
      }
    this->swap(*where, heapIndex_.size() - 1);
    heapIndex_.pop_back();
    heapIter_[i].reset();
    return siftDown(siftUp(*where));
  }
  /*!
   * @brief Update an element in the heap view.
   *
   * This function updates an element in the heap view.  It does not change the
   * position of the element in the data.  If the element is changed so that it
   * is no longer in the correct position in the heap, then the heap will be
   * rearranged
   * @note the value of the data element is changed
   * @param i The index in the data where we have to change the element
   * @param e The new value
   * @return The position in the hoep of the new value
   */
  Index
  update(DataIndex i, const ElementType &e)
  {
    data_[i] = e;
    auto where = heapIter_[i];
    if(not where.has_value())
      {
        // add again the value to the heap
        where = heapIndex_.size();
        heapIndex_.push_back(i);
        heapIter_[i] = where;
        return siftUp(*where);
      }
    return siftDown(siftUp(*where));
  }
  /*!
   * @brief The size of the heap
   * @return  The size
   */
  Index
  size() const
  {
    return heapIndex_.size();
  }
  /*!
   * The i-th element in the data
   * @param i The data index
   * @return the value
   */
  const ElementType &
  operator[](DataIndex i) const
  {
    return data_[i];
  }
  /*! The top element in the heap
   * @note This is the element at the top of the heap, not the element at the
   * top of the data.
   * @return the element value
   */
  const ElementType &
  top() const
  {
    return data_[heapIndex_[0]];
  }
  /*!
   * @brief The top element in the heap and its index in the data
   * @return The pair index, value
   */
  std::pair<DataIndex, DataElementType>
  topPair() const
  {
    return std::make_pair(heapIndex_[0], data_[heapIndex_[0]]);
  }
  /*!
   * Checks if the heap is empty
   * @return a boolean
   */
  [[nodiscard]] bool
  empty() const
  {
    return heapIndex_.empty();
  }
  /*!
   *  @brief Clear the heap
   */
  void
  clear()
  {
    data_.resize(0);
    heapIndex_.clear();
    heapIter_.clear();
  }
  /*!
   * @brief Pops the top element from the heap
   * @return The value of the top element
   * @details The top element is removed from the heap but NOT from the data
   */
  DataElementType
  pop()
  {
    return this->popPair().second;
  }

  /*!
   * @brief pops the top element from the heap
   * @return the index and value of the top element
   * @details The top element is removed from the heap but NOT from the data
   */
  std::pair<DataIndex, DataElementType>
  popPair()
  {
    auto const [where, e] = this->topPair();
    remove(where); // remove data element from the heap
    return std::make_pair(where, e);
  }
  /*!
   * @brief The data value at index i in the heap
   * @param i The index
   * @return The ith data value in the heap
   */
  auto
  heapValue(DataIndex i) const
  {
    return data_[heapIndex_[i]];
  }
  /* @brief The index of the data value in the heap
   * @param i The index in the data
   * @return The index in the heap
   */
  auto
  heapIndex(DataIndex i) const
  {
    return heapIndex_[i];
  }
  /*!
   * @brief Check iif the heap is ssane
   * @return  true is heap is sane
   */
  bool
  check() const
  {
    for(Index i = 0; i < heapIndex_.size(); ++i)
      {
        auto const [left, right] = children(i);
        if(left)
          {
            if(comp_(data_[heapIndex_[*left]], data_[heapIndex_[i]]))
              {
                return false;
              }
          }
        if(right)
          {
            if(comp_(data_[heapIndex_[*right]], data_[heapIndex_[i]]))
              {
                return false;
              }
          }
      }
    return true;
  }
  /*
  Comparing two elements using the given ordering relation
  */
  bool
  compare(DataIndex i, DataIndex j) const
  {
    return compHeapView_(i, j);
  }

private:
  /*!
   * @brief The parent of a node in the heap
   * @param i the index
   * @return the index of the parent
   */
  Index
  parent(Index i) const noexcept
  {
    return (i == 0u) ? 0u : (i - 1u) / 2u;
  };
  /*!
   * The indexex of the chirldens of a node in the heap
   * @param i The index
   * @return A pair of optionals.  The first is the left child, the second is
   * the right child.  If the child does not exist, the optional is empty.
   */
  std::pair<std::optional<Index>, std::optional<Index>>
  children(Index i) const noexcept
  {
    if(2 * i + 1u < heapIndex_.size())
      {
        if(2 * i + 2u < heapIndex_.size())
          {
            return std::make_pair(std::optional<Index>(2 * i + 1u),
                                  std::optional<Index>(2 * i + 2u));
          }
        else
          {
            return std::make_pair(std::optional<Index>(2 * i + 1u),
                                  std::optional<Index>());
          }
      }
    else
      {
        return std::make_pair(std::optional<Index>(), std::optional<Index>());
      }
  }
  /*!
   * Swap two elements in the heap
   *
   * Data is untouched. Only th eheap index are swappend and the heap iterators
   * are updated
   * @param i The first element
   * @param j The second element
   */
  void
  swap(Index i, Index j)
  {
    std::swap(heapIndex_[i], heapIndex_[j]);
    heapIter_[heapIndex_[i]] = i;
    heapIter_[heapIndex_[j]] = j;
  }
  //! Moves an element up the heap until it is in the correct position
  Index
  siftUp(Index i)
  {
    while(i > 0 && compHeapView_(heapIndex_[i], heapIndex_[parent(i)]))
      {
        this->swap(i, parent(i));
        i = parent(i);
      }
    return i;
  }
  //! Moves an element down the heap until it is in the correct position
  Index
  siftDown(Index i)
  {
    auto [left, right] = children(i);
    while(left)
      {
        Index j = *left;
        if(right && compHeapView_(heapIndex_[*right], heapIndex_[*left]))
          {
            j = *right;
          }
        if(compHeapView_(heapIndex_[j], heapIndex_[i]))
          {
            this->swap(i, j);
            i = j;
            std::tie(left, right) = children(i);
          }
        else
          {
            break;
          }
      }
    return i;
  }

  //! The data
  DataVector data_;
  //! The heap index
  HeapIndex heapIndex_;
  //! The heap iterators. In fact, it is a reverse map from data index to heap
  //! index
  HeapIter heapIter_;
  //! The comparison operator for the heap
  bool
  compHeapView_(DataIndex i, DataIndex j) const
  {
    return comp_(data_[i], data_[j]);
  };
  //! The comparison operator for the data (user defined or defaulted to less)
  CompOp comp_ = CompOp{};
  // static auto constexpr noData=std::numeric_limits<std::size_t>::max();
};
} // namespace apsc

#endif // EXAMPLES_HEAPVIEW_HPP
