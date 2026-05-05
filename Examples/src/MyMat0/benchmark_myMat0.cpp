#include "MyMat0.hpp"
#include "MyMat0_util.hpp"

#include <benchmark/benchmark.h>

namespace
{
using LinearAlgebra::COLUMNMAJOR;
using LinearAlgebra::MyMat0;
using LinearAlgebra::ROWMAJOR;

// Utility used by all benchmarks: build a matrix of the requested size and
// fill it with deterministic pseudo-random values so that runs are
// reproducible.
template <LinearAlgebra::StoragePolicySwitch Policy>
MyMat0<double, Policy>
makeMatrix(std::size_t rows, std::size_t cols, unsigned int seed)
{
  MyMat0<double, Policy> m(rows, cols);
  m.fillRandom(seed);
  return m;
}

// Benchmark matrix-vector multiplication when the matrix is stored by rows.
// This is the most natural traversal pattern for the implemented algorithm.
static void
BM_VecMultiplyRowMajor(benchmark::State &state)
{
  auto const n = static_cast<std::size_t>(state.range(0));
  auto matrix = makeMatrix<ROWMAJOR>(n, n, 1234U);
  std::vector<double> vec(n, 1.0);
  std::vector<double> result;

  for(auto _ : state)
    {
      // The operation under test is executed inside the benchmark loop.
      matrix.vecMultiply(vec, result);
      // Prevent the compiler from optimizing the result away.
      benchmark::DoNotOptimize(result);
    }

  state.SetComplexityN(state.range(0));
}

// Same experiment as above, but now the matrix is stored by columns.  This
// lets students compare the effect of the storage policy on the same logical
// operation.
static void
BM_VecMultiplyColumnMajor(benchmark::State &state)
{
  auto const n = static_cast<std::size_t>(state.range(0));
  auto matrix = makeMatrix<COLUMNMAJOR>(n, n, 1234U);
  std::vector<double> vec(n, 1.0);
  std::vector<double> result;

  for(auto _ : state)
    {
      matrix.vecMultiply(vec, result);
      benchmark::DoNotOptimize(result);
    }

  state.SetComplexityN(state.range(0));
}

// Generic driver for matrix-matrix benchmarks.  It removes repetitive setup
// code so each benchmark can focus on the multiplication variant being timed.
template <typename LeftMatrix, typename RightMatrix, typename Multiply>
void
runMatMulBenchmark(benchmark::State &state, Multiply multiply)
{
  auto const n = static_cast<std::size_t>(state.range(0));
  auto left = LeftMatrix(n, n);
  auto right = RightMatrix(n, n);
  left.fillRandom(1234U);
  right.fillRandom(5678U);

  for(auto _ : state)
    {
      // Create the result inside the loop so the measured time includes the
      // full multiplication cost seen by a caller.
      auto result = multiply(left, right);
      benchmark::DoNotOptimize(result);
      benchmark::ClobberMemory();
    }

  state.SetComplexityN(state.range(0));
}

// Baseline triple-loop implementation.  The other benchmarks can be read as
// progressively more cache-aware alternatives to this one.
static void
BM_MatMul_RowRow(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, ROWMAJOR>, MyMat0<double, ROWMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMul(a, b); });
}

// Optimized multiplication with both matrices stored row-major.
static void
BM_MatMulOpt_RowRow(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, ROWMAJOR>, MyMat0<double, ROWMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOpt(a, b); });
}

// Optimized multiplication for the row-major/column-major case, typically one
// of the best combinations because rows and columns are both contiguous.
static void
BM_MatMulOpt_RowColumn(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, ROWMAJOR>, MyMat0<double, COLUMNMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOpt(a, b); });
}

// Optimized multiplication when both matrices are stored column-major.
static void
BM_MatMulOpt_ColumnColumn(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, COLUMNMAJOR>, MyMat0<double, COLUMNMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOpt(a, b); });
}

// Optimized multiplication for the least favorable layout combination in this
// implementation.  Including it is useful for performance comparisons.
static void
BM_MatMulOpt_ColumnRow(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, COLUMNMAJOR>, MyMat0<double, ROWMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOpt(a, b); });
}

#ifndef NOBLAS
// BLAS-based benchmark with row-major storage for both operands.
static void
BM_MatMulBlas_RowRow(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, ROWMAJOR>, MyMat0<double, ROWMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOptBlas(a, b); });
}

// BLAS-based benchmark for row-major times column-major.
static void
BM_MatMulBlas_RowColumn(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, ROWMAJOR>, MyMat0<double, COLUMNMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOptBlas(a, b); });
}

// BLAS-based benchmark for column-major times column-major.
static void
BM_MatMulBlas_ColumnColumn(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, COLUMNMAJOR>, MyMat0<double, COLUMNMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOptBlas(a, b); });
}

// BLAS-based benchmark for column-major times row-major.
static void
BM_MatMulBlas_ColumnRow(benchmark::State &state)
{
  runMatMulBenchmark<MyMat0<double, COLUMNMAJOR>, MyMat0<double, ROWMAJOR>>(
    state, [](auto const &a, auto const &b) { return LinearAlgebra::matMulOptBlas(a, b); });
}
#endif
} // namespace

// Register the vector benchmarks on a small range of square sizes.  The range
// is wide enough to show the growth trend while still being fast to run in a
// teaching example.
BENCHMARK(BM_VecMultiplyRowMajor)
  ->RangeMultiplier(2)
  ->Range(64, 512)
  ->Complexity();
BENCHMARK(BM_VecMultiplyColumnMajor)
  ->RangeMultiplier(2)
  ->Range(64, 512)
  ->Complexity();

// Register the matrix-matrix benchmarks.  The maximum size is kept moderate so
// that `make benchmark` remains practical in a classroom setting.
BENCHMARK(BM_MatMul_RowRow)->RangeMultiplier(2)->Range(64, 256)->Complexity();
BENCHMARK(BM_MatMulOpt_RowRow)->RangeMultiplier(2)->Range(64, 256)->Complexity();
BENCHMARK(BM_MatMulOpt_RowColumn)->RangeMultiplier(2)->Range(64, 256)->Complexity();
BENCHMARK(BM_MatMulOpt_ColumnColumn)
  ->RangeMultiplier(2)
  ->Range(64, 256)
  ->Complexity();
BENCHMARK(BM_MatMulOpt_ColumnRow)->RangeMultiplier(2)->Range(64, 256)->Complexity();

#ifndef NOBLAS
BENCHMARK(BM_MatMulBlas_RowRow)->RangeMultiplier(2)->Range(64, 256)->Complexity();
BENCHMARK(BM_MatMulBlas_RowColumn)
  ->RangeMultiplier(2)
  ->Range(64, 256)
  ->Complexity();
BENCHMARK(BM_MatMulBlas_ColumnColumn)
  ->RangeMultiplier(2)
  ->Range(64, 256)
  ->Complexity();
BENCHMARK(BM_MatMulBlas_ColumnRow)
  ->RangeMultiplier(2)
  ->Range(64, 256)
  ->Complexity();
#endif

BENCHMARK_MAIN();
