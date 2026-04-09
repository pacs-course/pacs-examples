# A Tool for Block Matrices #

With this tool you can create block matrices of any size whose blocks are
Eigen sparse matrices.

## How Does It Work? ##
You can construct a block matrix `M` from its constituent blocks `M_ij`.

First, construct an empty block matrix by specifying the block sizes. In
particular, you have to provide the number of rows and columns of each block.

Then you can add the actual sparse matrices, possibly by moving them. You may
also indicate that a matrix block is the transpose of another block, without
explicitly creating the transpose.

The class also provides methods for

- Accessing a given block;
- Matrix-vector multiplication `M*v`;
- Vector-matrix multiplication `v*M` (`v` is a row vector);
- Computing the Frobenius norm;
- Extracting the complete sparse matrix as a single matrix.

The blocks are stored using *shared pointers to Eigen matrices*. This is the
mechanism used to avoid storing the transpose of a given block. A transposed
block is just a pointer to the original block, and the class keeps track of
the fact that the block represents a transpose.

This implementation relies on the fact that Eigen matrices support move
semantics, so use `std::move` if you want to move a block into the block
matrix rather than copy it.
 
Some checks verify that the dimensions of a stored block match the block size
allocated during construction.

## What Do I Learn Here? ##
- Some features of the Eigen library;
- One use of `shared_ptr`.

## Detailed Description of `SparseBlockMatrix.hpp` ##

The class template

```cpp
template <typename T, unsigned int M, unsigned int N,
          int storageOrder = Eigen::ColMajor>
class SparseBlockMatrix;
```

represents a sparse block matrix with:

- `T`: the scalar type stored in the matrix, for example `double`;
- `M`: the number of block rows;
- `N`: the number of block columns;
- `storageOrder`: the Eigen sparse storage order, by default
  `Eigen::ColMajor`.

Each block is an Eigen sparse matrix. Internally, the class stores one sparse
matrix for each block and keeps track of:

- The size of each block row;
- The size of each block column;
- The offsets needed to map local block indices to global matrix indices;
- Whether a block is an actual matrix or just the transpose of another block.

The most useful internal type aliases are:

```cpp
using SpMat = Eigen::SparseMatrix<T, storageOrder>;
using ColVector = Eigen::Matrix<T, Eigen::Dynamic, 1>;
using RowVector = Eigen::Matrix<T, 1, Eigen::Dynamic>;
using Index = Eigen::Index;
```

There is also a small helper structure used to identify a block:

```cpp
struct Indexes
{
  Index row;
  Index col;
};
```

This means that blocks can be addressed with a compact syntax such as
`{0, 1}` or `Indexes{1, 0}`.

### Construction and Initialization ###

There are two ways to create a block matrix.

You can construct it directly with block sizes:

```cpp
// Two block rows of sizes 2 and 4, and two block columns of sizes 2 and 4.
apsc::SparseBlockMatrix<double, 2, 2> A({2, 4}, {2, 4});
```

or you can default-construct it and initialize it later:

```cpp
apsc::SparseBlockMatrix<double, 2, 2> A;
A.resize({2, 4}, {2, 4});
```

In both cases:

- block `{0, 0}` has size `2 x 2`;
- block `{0, 1}` has size `2 x 4`;
- block `{1, 0}` has size `4 x 2`;
- block `{1, 1}` has size `4 x 4`.

At construction time all blocks are zero sparse matrices.

### Setting and Accessing Blocks ###

You can fill a block by first building an Eigen sparse matrix and then storing
it with `setBlock()`.

```cpp
using BlockMatrix = apsc::SparseBlockMatrix<double, 2, 2>;
using Tri = Eigen::Triplet<double>;

BlockMatrix A({2, 4}, {2, 4});
BlockMatrix::SpMat A00(A.rows({0, 0}), A.cols({0, 0}));

std::vector<Tri> coeff;
coeff.emplace_back(0, 0, 3.0);
coeff.emplace_back(0, 1, 9.0);
A00.setFromTriplets(coeff.begin(), coeff.end());

// Copy the sparse matrix into block (0,0).
A.setBlock({0, 0}, A00);
```

If you already want direct access to the stored block, you can use
`getBlock()`:

```cpp
coeff.clear();
coeff.emplace_back(0, 0, 1.0);
coeff.emplace_back(0, 1, 2.0);
coeff.emplace_back(1, 0, 3.0);

// Directly modify block (0,1).
A.getBlock({0, 1}).setFromTriplets(coeff.begin(), coeff.end());
```

`setBlock()` is safer because it checks block dimensions. `getBlock()` is more
direct and efficient, but it assumes that the caller already knows that the
block dimensions are correct.

### Representing a Transposed Block ###

One interesting feature of this class is that a block can be declared as the
transpose of another block without explicitly building and storing the
transposed sparse matrix.

```cpp
// Block (1,0) is represented as the transpose of block (0,1).
A.addTranspose({1, 0}, {0, 1});
```

This is useful in block structures that are symmetric or saddle-point like.
The class stores only one sparse matrix and lets the second block share that
data.

When you call `getBlock({1, 0})`, you receive the underlying stored matrix, so
you must remember that the logical block is transposed:

```cpp
// Print the logical content of block (1,0).
std::cout << A.getBlock({1, 0}).transpose() << std::endl;
```

You can check this state with:

```cpp
if (A.isTranspose({1, 0}))
{
  std::cout << "Block (1,0) is stored as a transpose view\n";
}
```

### Global Sizes and Offsets ###

The functions `rows()` and `cols()` return the size of the full block matrix:

```cpp
std::cout << "Global rows: " << A.rows() << '\n';
std::cout << "Global cols: " << A.cols() << '\n';
```

while

- `rows({i, j})` returns the number of rows of block `{i, j}`;
- `cols({i, j})` returns the number of columns of block `{i, j}`;
- `rowOffset(i)` returns the starting row of block row `i` in the full matrix;
- `colOffset(j)` returns the starting column of block column `j` in the full
  matrix.

These offsets are used internally when the class assembles the global matrix or
performs matrix-vector products.

### Matrix-Vector Products ###

The header defines two overloaded operators:

- `A * x` for a column vector;
- `y * A` for a row vector.

Example:

```cpp
BlockMatrix::ColVector x = BlockMatrix::ColVector::Ones(A.cols());
auto res1 = A * x;

BlockMatrix::RowVector y = BlockMatrix::RowVector::Ones(A.rows());
auto res2 = y * A;
```

These operators work block by block. If a block is marked as a transpose view,
the class automatically uses the transpose during multiplication.

This is exactly the behavior demonstrated in `main_SparseBlockMatrix.cpp`,
where the result of the block computation is compared with the result obtained
from the fully assembled sparse matrix.

### Building the Full Sparse Matrix ###

If needed, you can assemble the complete Eigen sparse matrix:

```cpp
auto F = A.fullMatrix();
std::cout << "Full Matrix =\n" << F << std::endl;
```

This operation is convenient for debugging and verification, but it is more
expensive than working directly with the block representation.

Internally, `fullMatrix()` scans all blocks, converts local nonzero entries
into global coordinates using row and column offsets, and finally creates a
single Eigen sparse matrix from a triplet list.

### Norms and Utility Functions ###

The class provides a few additional utilities:

- `nonZeros()` returns the total number of stored nonzero coefficients;
- `squaredNorm()` returns the squared Frobenius norm;
- `norm()` returns the Frobenius norm;
- `reserve(block, nnz)` reserves storage for a given block;
- `makeCompressed()` compresses all non-transposed stored matrices;
- `clearBlock(block)` resets one block to the zero matrix while preserving its
  dimensions;
- `clear()` resets the whole block matrix to zero-sized blocks.

Example:

```cpp
std::cout << "Frobenius norm: " << A.norm() << '\n';
std::cout << "Stored nonzeros: " << A.nonZeros() << '\n';

// Remove the content of block (0,0), but keep its dimensions.
A.clearBlock({0, 0});
```

### Copy and Shared Storage ###

The test program also shows that the block matrix can be copied:

```cpp
auto B = A;
```

This is important because the class stores blocks through `std::shared_ptr`.
As a consequence, copying the block matrix copies the block structure and the
shared ownership of the underlying sparse matrices.

This design is especially useful for transpose blocks, because two logical
blocks may intentionally refer to the same stored sparse matrix.

### A Minimal Complete Example ###

The following example summarizes the typical workflow:

```cpp
#include "SparseBlockMatrix.hpp"
#include <Eigen/Sparse>
#include <vector>

int main()
{
  using BlockMatrix = apsc::SparseBlockMatrix<double, 2, 2>;
  using Tri = Eigen::Triplet<double>;

  BlockMatrix A({2, 4}, {2, 4});

  // Build block (0,0).
  BlockMatrix::SpMat A00(A.rows({0, 0}), A.cols({0, 0}));
  std::vector<Tri> coeff{{0, 0, 3.0}, {0, 1, 9.0}};
  A00.setFromTriplets(coeff.begin(), coeff.end());
  A.setBlock({0, 0}, A00);

  // Build block (0,1) directly in the stored matrix.
  coeff = {{0, 0, 1.0}, {0, 1, 2.0}, {1, 0, 3.0}};
  A.getBlock({0, 1}).setFromTriplets(coeff.begin(), coeff.end());

  // Reuse the same data to represent block (1,0) as a transpose.
  A.addTranspose({1, 0}, {0, 1});

  // Multiply by a vector of ones.
  BlockMatrix::ColVector x = BlockMatrix::ColVector::Ones(A.cols());
  auto b = A * x;

  // Assemble the full matrix only if needed.
  auto F = A.fullMatrix();

  return 0;
}
```

This example shows the intended programming model:

- define the block layout;
- fill some blocks explicitly;
- reuse blocks through transposition when appropriate;
- work with the block matrix directly for algebraic operations;
- assemble the full matrix only when needed for inspection or validation.
