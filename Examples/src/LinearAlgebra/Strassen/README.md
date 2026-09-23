# Strassen Matrix Multiplication with Eigen

This folder contains a small implementation of Strassen's algorithm for dense
matrix-matrix multiplication using Eigen matrices, together with a benchmark
driver and a plotting script.

The main goal of the example is not to replace Eigen's optimized product in
general, but to show:

- how a recursive matrix multiplication algorithm can be implemented on top of
  Eigen;
- when such an algorithm may or may not be convenient in practice;
- how to benchmark a custom implementation against Eigen's standard `A * B`
  product.

## What Is Strassen's Algorithm?

The classical multiplication of two dense `n x n` matrices requires
`O(n^3)` arithmetic operations.

Strassen's idea is to split the matrices into four blocks

```text
A = [A11 A12]   B = [B11 B12]
    [A21 A22]       [B21 B22]
```

and compute the product by recursively forming seven block products instead of
eight. The reduction in the number of multiplications lowers the asymptotic
cost to about

```text
O(n^(log2(7))) ~= O(n^2.807)
```

at the price of:

- more additions and subtractions;
- more temporary storage;
- more complex memory access patterns;
- a strong dependence on the recursion cutoff.

For real implementations, this matters a lot: asymptotically fewer
multiplications do not automatically mean faster execution on modern hardware.
Highly tuned libraries such as Eigen already use cache blocking, vectorization,
and optimized kernels, so Strassen becomes interesting only in selected
regimes and must be benchmarked carefully.

## Files in This Folder

- [`strassen.hpp`](./strassen.hpp)
  A hybrid Strassen/Eigen implementation. It falls back to Eigen's optimized
  product for cases where recursion is not expected to help.

- [`main_StrassenBenchmark.cpp`](./main_StrassenBenchmark.cpp)
  A benchmark program that compares `apsc::strassen()` with standard Eigen
  multiplication for several matrix sizes and cutoff values.

- [`plot_strassen_benchmark.py`](./plot_strassen_benchmark.py)
  A small Python script that reads the benchmark output and generates a plot.

- [`Makefile`](./Makefile)
  Build file for the benchmark executable.

## How the Implementation Is Organized

The public function is:

```cpp
apsc::strassen(A, B, cutoff)
```

where `A` and `B` are Eigen dense matrices or Eigen matrix expressions, and
`cutoff` is the recursion threshold.

The implementation follows a hybrid strategy:

- it evaluates the input expressions once into owned dense matrices;
- it uses Eigen's direct product as the base kernel;
- it applies Strassen recursion when the square problem obtained by padding is
  larger than the cutoff;
- it handles odd and rectangular compatible products by zero-padding the
  operands to a square size, running the recursive kernel, and cropping the
  result back to the requested shape.

This is an important design choice. For many sizes, the fastest code path is
still simply:

```cpp
C.noalias() = A * B;
```

The padding makes the algorithm more general, but it can also add work and
temporary storage, especially for very rectangular matrices. Therefore this
example is best read as a performance study rather than as a universal
replacement for Eigen multiplication.

## How to Compile the Benchmark

Move to this directory:

```bash
cd Examples/src/LinearAlgebra/Strassen
```

Then build in optimized mode:

```bash
make DEBUG=no
```

This produces the executable:

```text
main_StrassenBenchmark
```

## How to Run the Benchmark

The benchmark tests:

- several square matrix sizes;
- several cutoff values for the Strassen recursion;
- Eigen's standard product as the reference implementation.

Default run:

```bash
./main_StrassenBenchmark
```

Custom run:

```bash
./main_StrassenBenchmark \
  --sizes=128,256,512,1024 \
  --cutoffs=32,64,128,256,512 \
  --repetitions=5
```

The output is a text table with:

- matrix size;
- method (`Eigen` or `Strassen`);
- cutoff value;
- minimum, median, and maximum execution time in milliseconds;
- speedup ratio `Eigen time / Strassen time`;
- relative error with respect to the Eigen result.

The benchmark now writes the measurements in CSV format, with one row for
each tested case:

```text
size,method,cutoff,min_ms,median_ms,max_ms,speedup,rel_error
```

This makes it easier to post-process the results and ensures that the plotting
script receives the complete dataset, not only a simplified textual summary.

If the speedup is:

- larger than `1`, Strassen is faster than Eigen for that case;
- smaller than `1`, Eigen is faster.

## How to Save and Plot the Results

Save the benchmark output to a file:

```bash
./main_StrassenBenchmark \
  --sizes=128,256,512,1024 \
  --cutoffs=32,64,128,256,512 \
  --repetitions=5 > benchmark.out
```

Then generate the plot:

```bash
python3 plot_strassen_benchmark.py benchmark.out --output benchmark.png
```

The plotting script generates:

- an Eigen timing plot versus matrix size;
- a Strassen median-time heatmap for all size/cutoff pairs;
- a speedup heatmap for all size/cutoff pairs;
- a relative-error heatmap for all size/cutoff pairs.

In this way, the image contains all benchmarked results.

The script requires `matplotlib`.

## What to Learn from This Example

- how recursive dense linear algebra algorithms can be mapped to Eigen blocks;
- why asymptotic complexity is not the only criterion for performance;
- why cutoff tuning is essential in hybrid recursive algorithms;
- how to benchmark numerical kernels in a reproducible way;
- how to compare algorithmic ideas against a highly optimized library baseline.

## Main References

1. V. Strassen, *Gaussian Elimination is Not Optimal*, Numerische Mathematik,
   13, 354-356, 1969.

2. G. H. Golub and C. F. Van Loan, *Matrix Computations*, 4th edition, Johns
   Hopkins University Press, 2013.

3. D. E. Knuth, *The Art of Computer Programming*, Volume 2:
   *Seminumerical Algorithms*, 3rd edition, Addison-Wesley, 1997.

4. Eigen documentation:
   [https://eigen.tuxfamily.org](https://eigen.tuxfamily.org)

For a theory note that also documents the implementation choices in this
folder, see [StrassenTheory.tex](StrassenTheory.tex).

For performance-oriented discussions of fast matrix multiplication in practice,
it is also worth consulting the numerical linear algebra literature on cache
effects, blocking, and communication costs. The main lesson is that a lower
asymptotic operation count is only one part of the performance picture.
