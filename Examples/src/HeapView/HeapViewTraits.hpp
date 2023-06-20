//
// Created by forma on 20/06/23.
//

#ifndef EXAMPLES_HEAPVIEWTRAITS_HPP
#define EXAMPLES_HEAPVIEWTRAITS_HPP
#include <vector>
namespace apsc
{
template <class DataElementType> struct heapViewTraits
{
  using Index = std::size_t;
  using DataType = std::vector<DataElementType>;
  using DataIndex = std::size_t;
  using ElementType = DataElementType;
  using HeapIndex = std::vector<Index>;
  using HeapIter = std::vector<Index>;
};
} // namespace apsc
#endif // EXAMPLES_HEAPVIEWTRAITS_HPP
