//
// Created by forma on 20/06/23.
//

#ifndef EXAMPLES_HEAPVIEWTRAITS_HPP
#define EXAMPLES_HEAPVIEWTRAITS_HPP
#include <vector>

namespace apsc
{

/*!
@brief Traits for the heapView class
*/
template <class DataElementType>
struct heapViewTraits
{
  //! The type of the index
  using Index = typename std::vector<DataElementType>::size_type;
  //! The type of the vector storing the data
  using DataVector = std::vector<DataElementType>;
  //! The type of the index vector
  using DataIndex = Index;
  /*! The type of the elements stored in the heap.
  * It is the same as the type of the elements stored in the data vector
  * @note We need a comparison operator on this type
  */ 
  using ElementType = DataElementType;
  //! The internal structure soring the indes of the heap
  using HeapIndex = std::vector<Index>;
  //! An internal structure storing the indexes of the data vector
  using HeapIter = std::vector<Index>;
};

} // namespace apsc
#endif // EXAMPLES_HEAPVIEWTRAITS_HPP
