//
// Created by forma on 20/06/23.
//

#ifndef EXAMPLES_HEAPVIEWTRAITS_HPP
#define EXAMPLES_HEAPVIEWTRAITS_HPP
#include <vector>
#include <optional>


namespace apsc
{

/*!
@brief Traits for the heapView class
@tparam DataElementType The type of the elements stored in the heap
*/
template <class DataElementType>
struct heapViewTraits
{
  //! The type of the index
  using Index = typename std::vector<DataElementType>::size_type;
  //! The type of the vector storing the data
  using DataVector = std::vector<DataElementType>;
  //! The type of the indixes of the heap (it is the same as the type of the indexes of the data vector)
  using DataIndex = Index;
  /*! The type of the elements stored in the heap.
  * It is the same as the type of the elements stored in the data vector
  * @note We need a comparison operator on this type
  */ 
  using ElementType = DataElementType;
  /*! The internal structure keeping the indexes of the heap
   * 
   * HeapIndex(i) returns the index of the i-th element in the heap
   */
  using HeapIndex = std::vector<Index>;
  /*! An internal structure storing the indexes of the data vector
   * HeapIter(i) returns the index of the i-th heap element in the data vector
   * We have HeapIndex(HeapIter(i))=i if i is a valid index of the heap 
  */
 using HeapIter = std::vector<std::optional<Index>>;
};

} // namespace apsc
#endif // EXAMPLES_HEAPVIEWTRAITS_HPP
