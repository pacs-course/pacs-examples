# RotatingMatrix.hpp - Rotating Matrix Data Structures

This code implements a "rotating matrix" data structure that extends the rotating vector concept to work with matrices using the Eigen linear algebra library. The core idea is to maintain a matrix that keeps only the most recently inserted columns, automatically removing older columns when the maximum capacity is reached.

## Core Concept

The rotating matrix acts like a **sliding window for matrix columns**, maintaining only the N most recently inserted columns. This is particularly useful for:
- Time series data analysis where you need a sliding window of recent observations
- Signal processing applications requiring a moving window of samples
- Machine learning algorithms that process sequential data in fixed-size batches
- Any scenario where you need to maintain recent history while discarding older data automatically

## Insert Strategies

The code provides two different strategies for handling column insertion when the matrix is full, controlled by the `InsertStrategy` enum:

### 1. `Shift` Strategy (Default)
- **Behavior**: Maintains chronological order by shifting all existing columns one position to the left and inserting the new column at the rightmost position
- **Pros**: Preserves insertion order - column indices correspond to insertion order
- **Cons**: Requires O(N) column copy operations on each insertion when full
- **Use case**: When column ordering matters and performance is secondary

### 2. `NewestReplacesOldest` Strategy  
- **Behavior**: Uses a circular buffer approach where new columns directly replace the oldest column in place
- **Pros**: O(1) insertion performance - very efficient
- **Cons**: Breaks chronological ordering of columns in memory
- **Use case**: When performance is critical and you don't need sequential column ordering

## Implementation Classes

### `RotatingMatrix<T, M, N, Strategy>`

The **compile-time fixed-size implementation** with template parameters:
- `T`: Element type (typically `double` or `float`)
- `M`: Number of rows (fixed at compile time)
- `N`: Maximum number of columns (fixed at compile time)
- `Strategy`: Insert strategy (defaults to `Shift`)

#### Key Features:
```cpp
// Type aliases for convenience
using Vector = Eigen::Matrix<T, M, 1>;        // Column vector type
using Matrix = Eigen::Matrix<T, M, N, Eigen::ColMajor>;  // Full matrix type

// Essential for Eigen fixed-size matrices as class members
EIGEN_MAKE_ALIGNED_OPERATOR_NEW
```

#### State Management:
- `N_size`: Tracks current number of inserted columns (0 to N)
- `oldestCol`: Points to the next column to replace (for `NewestReplacesOldest` strategy)
- `M_vec`: The underlying Eigen matrix storage

### `RotatingMatrixX<T, Strategy>`

The **runtime flexible implementation** that allows matrix dimensions to be set during construction:
- Uses `Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>` for dynamic sizing
- Stores dimensions in member variables `M` and `N`
- Useful when matrix dimensions aren't known until runtime
- Comes with dynamic memory allocation overhead

## Core Operations

### Column Insertion (`push_back`)
```cpp
void push_back(const Vector &arg)
{
    if(N > N_size) {
        // Matrix not full - simply append
        M_vec.col(N_size) = arg;
        ++N_size;
    } else {
        // Matrix full - apply strategy
        if constexpr(Strategy == InsertStrategy::Shift) {
            // Shift all columns left, insert at end
            for(std::size_t i = 0u; i < N_size - 1u; ++i)
                M_vec.col(i) = M_vec.col(i + 1);
            M_vec.col(N - 1) = arg;
        } else {
            // Replace oldest column directly
            M_vec.col(oldestCol) = arg;
            oldestCol = (oldestCol + 1u) % N;
        }
    }
}
```

### Matrix Access Methods
- **`getEigenFullMatrix()`**: Returns the complete underlying Eigen matrix (all N columns)
- **`getMatrix()`**: Returns only the "active" portion (first N_size columns) using Eigen's block operations
- **`col(j)`**: Access individual columns as Eigen vectors
- **Standard container interface**: `empty()`, `full()`, `cols()`, `max_size()`, `reset()`

## Eigen Integration

Both implementations provide extensive integration with the Eigen ecosystem:

### Direct Eigen Access
```cpp
auto& fullMatrix = rotMatrix.getEigenFullMatrix();  // Full N-column matrix
auto activeMatrix = rotMatrix.getMatrix();          // Only active columns (block view)
auto column = rotMatrix.col(i);                     // i-th column as Eigen vector
```

### Memory Alignment
The `EIGEN_MAKE_ALIGNED_OPERATOR_NEW` macro ensures proper memory alignment for Eigen's vectorized operations, which is crucial for performance with fixed-size matrices.

### Column-Major Storage
Both classes use `Eigen::ColMajor` storage, which is optimal for column-wise operations and matches the typical use case of inserting entire columns.

## Performance Considerations

| Aspect | `Shift` Strategy | `NewestReplacesOldest` Strategy |
|--------|------------------|--------------------------------|
| **Insertion Time** | O(N) when full | O(1) always |
| **Memory Access** | Sequential column copying | Direct single column access |
| **Column Ordering** | Preserved (index = insertion order) | Circular (oldest wraps around) |
| **Cache Performance** | Poor when full (many copies) | Excellent (single write) |

### When to Use Each Strategy

**Use `Shift` Strategy when:**
- Column ordering/chronology is important
- Matrix size N is small (< 50 columns typically)
- Code clarity is more important than performance
- You need intuitive column indexing

**Use `NewestReplacesOldest` Strategy when:**
- Performance is critical
- Matrix size N is large
- You can handle non-sequential column ordering
- You're implementing real-time systems

## Common Use Cases

### Time Series Analysis
```cpp
// Maintain sliding window of last 10 time steps
RotatingMatrix<double, 100, 10> timeSeriesWindow;

// Add new observation column
Eigen::VectorXd newObservation(100);
// ... fill newObservation ...
timeSeriesWindow.push_back(newObservation);

// Process active window
auto currentWindow = timeSeriesWindow.getMatrix();
// ... perform analysis on currentWindow ...
```

### Signal Processing  
```cpp
// Buffer for real-time signal processing
RotatingMatrix<float, 512, 64, InsertStrategy::NewestReplacesOldest> signalBuffer;

// Add new sample frame
signalBuffer.push_back(newSampleFrame);

// Access for FFT or filtering
auto bufferData = signalBuffer.getEigenFullMatrix();
```

## Design Benefits

1. **Memory Efficiency**: Fixed memory footprint regardless of how many columns are inserted over time
2. **Eigen Compatibility**: Seamless integration with existing Eigen-based code
3. **Type Safety**: Compile-time guarantees for matrix dimensions (fixed-size version)
4. **Performance Options**: Choice between memory layout strategies based on use case
5. **Clean Interface**: Standard container-like methods with matrix-specific extensions

This design provides an efficient way to maintain recent matrix history with different trade-offs between compile-time constraints, runtime flexibility, and performance characteristics based on your specific use case requirements.

## A Note ##
The code can be further extended with additional features like:
- Move semantics for better performance with large matrices
- Implement row-wise rotating matrices