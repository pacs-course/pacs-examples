# HeapView: A Detailed Code Analysis

## Overview

The `heapView.hpp` file implements a sophisticated data structure called `HeapView` that provides heap operations on a collection of data without physically rearranging the original data in memory. This is particularly useful when you need heap-based operations (priority queue functionality) while maintaining the original data structure intact.

## Core Concept

### Traditional Heap vs HeapView

- **Traditional Heap**: Elements are directly stored in the heap structure, and heap operations move the actual data elements around
- **HeapView**: The original data remains untouched; instead, a heap of *indices* is maintained that points to the data elements

### Key Innovation

The `HeapView` maintains three main components:
1. **Data Vector** (`data_`): Stores the actual elements (unchanged by heap operations)
2. **Heap Index** (`heapIndex_`): A heap of indices pointing to data elements  
3. **Heap Iterator** (`heapIter_`): Reverse mapping from data indices to heap positions

## Class Template Declaration

```cpp
template <class DataElementType, class CompOp = std::less<DataElementType>,
          class Traits = heapViewTraits<DataElementType>>
class HeapView
```

### Template Parameters

- **`DataElementType`**: The type of elements stored in the data vector
- **`CompOp`**: Comparison operator (defaults to `std::less` for min-heap). In general, the top of the heap is the "smallest" element according to this comparator
- **`Traits`**: Type traits providing internal type definitions

## Type Definitions

The class uses the traits pattern to define internal types:

```cpp
using Index = Traits::Index;              // Index type (typically size_t)
using DataVector = Traits::DataVector;    // Container for data elements
using DataIndex = Traits::DataIndex;      // Index into data vector
using ElementType = Traits::ElementType;  // Type of stored elements
using HeapIndex = Traits::HeapIndex;      // Container for heap indices
using HeapIter = Traits::HeapIter;        // Container for reverse mapping
```

## Core Data Members

### Private Members

```cpp
DataVector data_;        // The actual data elements
HeapIndex heapIndex_;    // Heap of indices pointing to data elements  
HeapIter heapIter_;      // Reverse map: data index → heap position
CompOp comp_;            // Comparison operator for ordering
```

### Data Structure Relationships

The relationship between these structures is crucial:
- `heapIndex_[i]` gives the data index of the i-th element in the heap
- `heapIter_[j]` gives the heap position of data element j (if in heap)
- `data_[heapIndex_[i]]` gives the actual value of the i-th heap element

## Constructor and Initialization

### Primary Constructor

```cpp
template <class T>
explicit HeapView(T &&data, CompOp comp = CompOp{})
  : data_(std::forward<T>(data)), comp_(comp)
```

**Initialization Process:**
1. Store data using perfect forwarding
2. Reserve space for index structures
3. Initialize `heapIndex_` with sequential indices `[0, 1, 2, ..., n-1]`
4. Initialize `heapIter_` with the same sequential indices
5. **Heapify**: Convert the index array into a proper heap using bottom-up approach

### Heapification Algorithm

```cpp
// Bottom-up heapification
for(Index i = data_.size() / 2; i > 0; --i) {
    siftDown(i);
}
siftDown(0);  // Handle root separately to avoid unsigned underflow
```

**Why Bottom-Up?** Starting from the last non-leaf node ensures that when we process a node, its children are already valid heaps.

## Core Heap Operations

### Sift Up Operation

```cpp
Index siftUp(Index i) {
    while(i > 0 && compHeapView_(heapIndex_[i], heapIndex_[parent(i)])) {
        this->swap(i, parent(i));
        i = parent(i);
    }
    return i;
}
```

**Purpose**: Moves an element up the heap until heap property is satisfied
**When Used**: After inserting a new element or when an element's priority increases

### Sift Down Operation

```cpp
Index siftDown(Index i) {
    auto [left, right] = children(i);
    while(left) {
        Index j = *left;
        if(right && compHeapView_(heapIndex_[*right], heapIndex_[*left])) {
            j = *right;
        }
        if(compHeapView_(heapIndex_[j], heapIndex_[i])) {
            this->swap(i, j);
            i = j;
            std::tie(left, right) = children(i);
        } else {
            break;
        }
    }
    return i;
}
```

**Purpose**: Moves an element down the heap until heap property is satisfied
**Algorithm**: Always swap with the "smaller" child (according to comparison operator)

## Public Interface Methods

### Adding Elements

```cpp
template <class E>
Index add(E &&e) {
    Index i = this->size();
    data_.emplace_back(std::forward<E>(e));
    heapIndex_.emplace_back(data_.size() - 1u);
    heapIter_.emplace_back(i);
    return siftUp(i);
}
```

**Process:**
1. Add element to data vector
2. Add new data index to heap
3. Update reverse mapping
4. Restore heap property with sift-up

### Removing Elements

```cpp
Index remove(DataIndex i) {
    auto where = heapIter_[i];
    if(not where.has_value()) {
        throw std::invalid_argument("Element not in heap");
    }
    this->swap(*where, heapIndex_.size() - 1);
    heapIndex_.pop_back();
    heapIter_[i].reset();
    return siftDown(siftUp(*where));
}
```

**Key Points:**
- Element is removed from heap but **NOT** from data vector
- Uses `std::optional` in `heapIter_` to track removed elements
- Swap-and-pop technique for efficient removal
- Requires both sift-up and sift-down to restore heap property

### Updating Elements

```cpp
Index update(DataIndex i, const ElementType &e) {
    data_[i] = e;
    auto where = heapIter_[i];
    if(not where.has_value()) {
        // Re-add to heap if previously removed
        where = heapIndex_.size();
        heapIndex_.push_back(i);
        heapIter_[i] = where;
        return siftUp(*where);
    }
    return siftDown(siftUp(*where));
}
```

**Sophisticated Logic:**
- Updates the actual data value
- If element was previously removed, adds it back to heap
- Otherwise, restores heap property (element might need to move up or down)

## Advanced Features

### Optional-Based Tracking

The use of `std::optional<Index>` in `heapIter_` is elegant:
- `heapIter_[i].has_value()` indicates if data element `i` is currently in the heap
- `heapIter_[i].reset()` marks element as removed from heap
- Enables efficient re-insertion of previously removed elements

### Heap Navigation

```cpp
Index parent(Index i) const noexcept {
    return (i == 0u) ? 0u : (i - 1u) / 2u;
}

std::pair<std::optional<Index>, std::optional<Index>>
children(Index i) const noexcept {
    // Returns pair of optional children indices
}
```

**Binary Tree Properties:**
- Parent of node `i`: `(i-1)/2`
- Left child of node `i`: `2*i+1` 
- Right child of node `i`: `2*i+2`
- Uses `std::optional` for non-existent children

### Comparison Strategy

```cpp
bool compHeapView_(DataIndex i, DataIndex j) const {
    return comp_(data_[i], data_[j]);
}
```

The comparison is performed on the actual data values, not the indices. This allows the heap to order elements by their values while maintaining index-based structure.

## Memory Efficiency and Performance

### Space Complexity
- **Data Storage**: `O(n)` for original data
- **Heap Indices**: `O(n)` for heap structure  
- **Reverse Mapping**: `O(n)` for quick lookups
- **Total**: `O(n)` space (3× overhead for index management)

### Time Complexity
- **Insert**: `O(log n)` - requires sift-up
- **Remove**: `O(log n)` - requires sift-up + sift-down
- **Update**: `O(log n)` - requires sift-up + sift-down
- **Top Access**: `O(1)` - direct array access
- **Pop**: `O(log n)` - equivalent to remove + top

## Safety and Robustness

### Exception Safety
```cpp
if(not where.has_value()) {
    throw std::invalid_argument("HeapView: trying to remove an element not in the heap");
}
```

### Heap Validation
```cpp
bool check() const {
    for(Index i = 0; i < heapIndex_.size(); ++i) {
        auto const [left, right] = children(i);
        if(left && comp_(data_[heapIndex_[*left]], data_[heapIndex_[i]])) {
            return false;
        }
        if(right && comp_(data_[heapIndex_[*right]], data_[heapIndex_[i]])) {
            return false;
        }
    }
    return true;
}
```

This method verifies heap invariant: no child is "smaller" than its parent.

## Modern C++ Features Demonstrated

### 1. Perfect Forwarding
```cpp
template <class T>
explicit HeapView(T &&data, CompOp comp = CompOp{})
  : data_(std::forward<T>(data)), comp_(comp)
```

### 2. Structured Bindings (C++17)
```cpp
auto [left, right] = children(i);
auto const [where, e] = this->topPair();
```

### 3. Optional Types (C++17)
```cpp
std::optional<Index> left_child;
if(not where.has_value()) { /* handle missing element */ }
```

### 4. Template Type Deduction
```cpp
template <class E>
Index add(E &&e)  // Deduces E from argument type
```

### 5. Attributes (C++11/14)
```cpp
[[nodiscard]] bool empty() const  // Warns if return value ignored
```

## Use Cases and Applications

### 1. Priority Queues with Stable Data
When you need priority queue operations but want to preserve original data indexing:
```cpp
HeapView<Task> taskQueue(tasks);
while(!taskQueue.empty()) {
    auto [taskId, task] = taskQueue.popPair();
    processTask(taskId, task);
    // Original tasks vector unchanged
}
```

### 2. Dynamic Priority Updates
For algorithms where element priorities change:
```cpp
HeapView<double> priorities(distances);
priorities.update(nodeId, newDistance);  // Dijkstra's algorithm
```

### 3. Min-Heap vs Max-Heap
```cpp
HeapView<int> minHeap(data);                          // Default: min-heap
HeapView<int, std::greater<>> maxHeap(data);          // Max-heap
```

## Design Trade-offs

### Advantages
1. **Data Preservation**: Original data structure remains untouched
2. **Index Stability**: Data indices remain constant throughout operations
3. **Flexibility**: Easy to switch between different comparison operators
4. **Efficiency**: All operations are optimal `O(log n)`

### Disadvantages  
1. **Memory Overhead**: Requires 3× space for index management
2. **Complexity**: More complex than standard heap implementation
3. **Cache Performance**: Index indirection may impact cache efficiency

## Comparison with Standard Library

### vs `std::priority_queue`
- **Advantage**: Allows element removal and updates by index
- **Advantage**: Preserves original data structure
- **Disadvantage**: Higher memory usage and complexity

### vs `std::make_heap`
- **Advantage**: Non-destructive heap operations
- **Advantage**: Supports arbitrary element removal
- **Disadvantage**: Cannot operate directly on existing containers

## Example Usage Pattern

```cpp
#include "heapView.hpp"

int main() {
    std::vector<double> data{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};
    
    // Create min-heap view
    apsc::HeapView<double> heap(data);
    
    std::cout << "Top element: " << heap.top() << std::endl;  // 1.0
    
    // Remove minimum element from heap (but not from data)
    auto [index, value] = heap.popPair();
    std::cout << "Popped: index=" << index << ", value=" << value << std::endl;
    
    // Add new element
    heap.add(0.5);
    
    // Update existing element
    heap.update(2, 10.0);  // Change data[2] from 4.0 to 10.0
    
    // Verify heap property
    std::cout << "Heap is valid: " << heap.check() << std::endl;
}
```

## Educational Value

This implementation demonstrates several important computer science concepts:

### 1. **Data Structure Design**
- Separation of data storage from data organization
- Index-based heap implementation
- Reverse mapping for efficient updates

### 2. **Algorithm Efficiency**
- Bottom-up heapification
- Logarithmic time heap operations
- Space-time trade-offs

### 3. **Modern C++ Programming**
- Template metaprogramming with traits
- RAII and exception safety
- Perfect forwarding and move semantics
- Standard library integration

### 4. **Software Engineering**
- Trait-based design for customization  
- Clear separation of concerns
- Comprehensive error handling
- Self-documenting code with meaningful names

The `HeapView` class represents a sophisticated example of how to extend standard data structures while leveraging modern C++ features to create efficient, safe, and flexible code.