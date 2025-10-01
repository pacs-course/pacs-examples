# RotatingVector.hpp - Rotating Vector Data Structures

This header file implements three different variants of a **rotating vector** data structure - a container that maintains only the most recently inserted elements up to a fixed maximum capacity. When the container is full and a new element is added, the oldest element is automatically removed to make space.

## Core Concept

The rotating vector acts like a **circular buffer** with a FIFO (First In, First Out) behavior. Think of it as a sliding window that moves forward in time, keeping only the latest N elements. This is particularly useful in scenarios like:
- Maintaining a history of recent values
- Implementing moving averages
- Tracking the last few states in a system

## Three Implementations

### 1. `RotatingVector<T,N>`
The **basic implementation** using a compile-time fixed size N. It stores elements in an `std::array<T,N>` and uses `std::rotate` to shift elements when the container is full. This approach directly moves the actual objects, which is efficient for small or cheaply movable types.

### 2. `RotatingVector2<T,N>` 
Offers an **optimization for expensive-to-move objects**. Instead of rotating the actual elements, it maintains an array of iterators (`M_iter`) that point to the elements in `M_vec`. When rotation is needed, only the lightweight iterators are moved rather than the potentially heavy objects themselves. This reduces the cost of the rotation operation from O(N × sizeof(T)) to O(N × sizeof(iterator)).

### 3. `RotatingVectorXd<T>`
Provides **runtime flexibility** by allowing the maximum size to be set dynamically rather than at compile time. It uses `std::vector<T>` as the underlying storage and includes additional functionality like `setMaxSize()` to resize the container during execution.

## Key Operations

### Insertion Methods
- **`emplace_back()`** and **`push_back()`** implement the core rotating behavior
- When the container isn't full: new elements are simply added at the end
- Once full: `std::rotate` shifts all existing elements one position left (removing the front element) before adding the new element at the back

### Access Methods
- **Templated `back<i>()` methods** provide convenient access to recently inserted elements:
  - `back<0>()` returns the most recent element
  - `back<1>()` returns the second-most recent element
  - And so on...
- This compile-time indexing allows efficient access to historical values without runtime overhead

## Design Trade-offs

This design provides an efficient way to maintain recent history with different trade-offs:

| Implementation | Compile-time Size | Runtime Flexibility | Performance | Best For |
|----------------|-------------------|-------------------|-------------|----------|
| `RotatingVector` | ✅ | ❌ | High (small/movable types) | Simple use cases |
| `RotatingVector2` | ✅ | ❌ | High (large objects) | Expensive-to-move types |
| `RotatingVectorXd` | ❌ | ✅ | Moderate | Dynamic sizing needs |

The choice between implementations depends on whether you need compile-time constraints, runtime flexibility, and the move semantics characteristics of your element type.