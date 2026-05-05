#include "MyMat0.hpp"
#include "MyMat0_TypeTraitAndView/MyMat0_views.hpp"
#include "MyMat0_util.hpp"
#include <gtest/gtest.h>

#include <vector>

namespace
{
using LinearAlgebra::COLUMNMAJOR;
using LinearAlgebra::MyMat0;
using LinearAlgebra::ROWMAJOR;
using LinearAlgebra::TransposeView;

// Small reference matrix used in several tests.  The same logical matrix is
// instantiated with different storage layouts so that we can check that the
// public interface behaves identically.
template <LinearAlgebra::StoragePolicySwitch Policy>
MyMat0<double, Policy>
makeReferenceMatrix()
{
  MyMat0<double, Policy> m(2, 3);
  m(0, 0) = 1.0;
  m(0, 1) = -2.0;
  m(0, 2) = 3.0;
  m(1, 0) = 4.0;
  m(1, 1) = 5.0;
  m(1, 2) = -6.0;
  return m;
}

// Helper used to compare two matrices entry by entry.  We keep it separate
// from the individual tests so the intent of each test remains visible.
template <typename Matrix>
void
expectSameEntries(Matrix const &a, Matrix const &b)
{
  ASSERT_EQ(a.nrow(), b.nrow());
  ASSERT_EQ(a.ncol(), b.ncol());
  for(std::size_t i = 0; i < a.nrow(); ++i)
    for(std::size_t j = 0; j < a.ncol(); ++j)
      EXPECT_DOUBLE_EQ(a(i, j), b(i, j));
}

// Two small matrices with hand-computed products.  Using explicit values makes
// it easy to explain and verify the expected multiplication results.
template <LinearAlgebra::StoragePolicySwitch Policy>
MyMat0<double, Policy>
makeMulLeft()
{
  MyMat0<double, Policy> m(2, 3);
  m(0, 0) = 1.0;
  m(0, 1) = 2.0;
  m(0, 2) = 3.0;
  m(1, 0) = 4.0;
  m(1, 1) = 5.0;
  m(1, 2) = 6.0;
  return m;
}

template <LinearAlgebra::StoragePolicySwitch Policy>
MyMat0<double, Policy>
makeMulRight()
{
  MyMat0<double, Policy> m(3, 2);
  m(0, 0) = 7.0;
  m(0, 1) = 8.0;
  m(1, 0) = 9.0;
  m(1, 1) = 10.0;
  m(2, 0) = 11.0;
  m(2, 1) = 12.0;
  return m;
}
} // namespace

TEST(MyMat0Test, RowAndColumnMajorAccessAgree)
{
  // The same logical matrix should expose the same entries regardless of the
  // internal storage order.
  auto const rowMajor = makeReferenceMatrix<ROWMAJOR>();
  auto const columnMajor = MyMat0<double, COLUMNMAJOR>(rowMajor);

  ASSERT_EQ(rowMajor.getStoragePolicy(), ROWMAJOR);
  ASSERT_EQ(columnMajor.getStoragePolicy(), COLUMNMAJOR);
  EXPECT_EQ(rowMajor.getIndex(1, 2), 5U);
  EXPECT_EQ(columnMajor.getIndex(1, 2), 5U);
  expectSameEntries(rowMajor, MyMat0<double, ROWMAJOR>(columnMajor));
}

TEST(MyMat0Test, RowAndColumnExtractionAndReplacementWork)
{
  // This checks the helper methods that extract and overwrite complete rows
  // and columns.
  auto m = makeReferenceMatrix<ROWMAJOR>();

  EXPECT_EQ(m.row(0), (std::vector<double>{1.0, -2.0, 3.0}));
  EXPECT_EQ(m.col(1), (std::vector<double>{-2.0, 5.0}));

  m.replaceRow(1, std::vector<double>{7.0, 8.0, 9.0});
  m.replaceCol(0, std::vector<double>{10.0, 11.0});

  EXPECT_EQ(m.row(1), (std::vector<double>{11.0, 8.0, 9.0}));
  EXPECT_EQ(m.col(0), (std::vector<double>{10.0, 11.0}));
}

TEST(MyMat0Test, InvalidRowAndColumnReplacementThrows)
{
  // Size mismatches should be detected explicitly instead of silently writing
  // invalid data.
  auto m = makeReferenceMatrix<COLUMNMAJOR>();
  EXPECT_THROW(m.replaceRow(0, std::vector<double>{1.0, 2.0}), std::invalid_argument);
  EXPECT_THROW(m.replaceCol(0, std::vector<double>{1.0, 2.0, 3.0}),
               std::invalid_argument);
}

TEST(MyMat0Test, NormsAreComputedCorrectly)
{
  // Norms are tested on a matrix containing positive and negative entries so
  // that the use of absolute values is actually exercised.
  auto const m = makeReferenceMatrix<ROWMAJOR>();
  EXPECT_DOUBLE_EQ(m.norm1(), 9.0);
  EXPECT_DOUBLE_EQ(m.normInf(), 15.0);
  EXPECT_DOUBLE_EQ(m.normF(), std::sqrt(91.0));
}

TEST(MyMat0Test, VectorMultiplicationWorksForBothLayouts)
{
  // Matrix-vector multiplication should be independent of the internal storage
  // policy.
  auto const rowMajor = makeReferenceMatrix<ROWMAJOR>();
  auto const columnMajor = MyMat0<double, COLUMNMAJOR>(rowMajor);
  std::vector<double> const v{1.0, 2.0, 3.0};

  std::vector<double> resRow;
  std::vector<double> resColumn;
  rowMajor.vecMultiply(v, resRow);
  columnMajor.vecMultiply(v, resColumn);

  EXPECT_EQ(resRow, (std::vector<double>{6.0, -4.0}));
  EXPECT_EQ(resColumn, resRow);
}

TEST(MyMat0Test, VectorMultiplicationThrowsOnWrongSize)
{
  // A wrong vector size is a user error and should trigger the documented
  // exception path.
  auto const m = makeReferenceMatrix<ROWMAJOR>();
  std::vector<double> result;
  EXPECT_THROW(m.vecMultiply(std::vector<double>{1.0, 2.0}, result),
               std::invalid_argument);
  EXPECT_THROW(static_cast<void>(m * std::vector<double>{1.0, 2.0}),
               std::invalid_argument);
}

TEST(MyMat0Test, MatrixMultiplicationMatchesReferenceForAllLayouts)
{
  // We compute one reference product with the plain triple-loop algorithm and
  // then compare all optimized layout combinations against it.
  auto const rrLeft = makeMulLeft<ROWMAJOR>();
  auto const rrRight = makeMulRight<ROWMAJOR>();
  auto const rcRight = MyMat0<double, COLUMNMAJOR>(rrRight);
  auto const crLeft = MyMat0<double, COLUMNMAJOR>(rrLeft);

  auto const reference = LinearAlgebra::matMul(rrLeft, rrRight);
  EXPECT_DOUBLE_EQ(reference(0, 0), 58.0);
  EXPECT_DOUBLE_EQ(reference(0, 1), 64.0);
  EXPECT_DOUBLE_EQ(reference(1, 0), 139.0);
  EXPECT_DOUBLE_EQ(reference(1, 1), 154.0);

  expectSameEntries(reference, LinearAlgebra::matMulOpt(rrLeft, rrRight));
  expectSameEntries(reference, LinearAlgebra::matMulOpt(rrLeft, rcRight));
  expectSameEntries(reference, LinearAlgebra::matMulOpt(crLeft, rrRight));
  expectSameEntries(reference, LinearAlgebra::matMulOpt(crLeft, rcRight));
}

TEST(MyMat0Test, MatrixMultiplicationThrowsOnWrongSize)
{
  // Multiplication with incompatible dimensions must fail deterministically.
  MyMat0<double, ROWMAJOR> a(2, 3, 1.0);
  MyMat0<double, ROWMAJOR> b(4, 2, 1.0);
  EXPECT_THROW(static_cast<void>(LinearAlgebra::matMul(a, b)), std::invalid_argument);
  EXPECT_THROW(static_cast<void>(LinearAlgebra::matMulOpt(a, b)),
               std::invalid_argument);
}

TEST(MyMat0Test, ResizePreservesWhenElementCountMatches)
{
  // The current implementation preserves the raw data if the total number of
  // stored entries does not change.
  MyMat0<double, ROWMAJOR> m(2, 3);
  for(std::size_t i = 0; i < m.nrow(); ++i)
    for(std::size_t j = 0; j < m.ncol(); ++j)
      m(i, j) = static_cast<double>(i * m.ncol() + j + 1);

  m.resize(3, 2);

  EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(m(2, 1), 6.0);
}

TEST(MyMat0Test, TransposeViewReflectsUnderlyingMatrix)
{
  // The transpose view should swap dimensions, redirect indexing, and still
  // operate on the original matrix storage.
  MyMat0<int, ROWMAJOR> m(2, 3);
  m(0, 0) = 1;
  m(0, 1) = 2;
  m(0, 2) = 3;
  m(1, 0) = 4;
  m(1, 1) = 5;
  m(1, 2) = 6;

  TransposeView<MyMat0<int, ROWMAJOR>> view{m};
  EXPECT_EQ(view.nrow(), 3U);
  EXPECT_EQ(view.ncol(), 2U);
  EXPECT_EQ(view(2, 1), 6);

  view(1, 0) = 20;
  EXPECT_EQ(m(0, 1), 20);
  EXPECT_DOUBLE_EQ(view.norm1(), m.normInf());
  EXPECT_DOUBLE_EQ(view.normInf(), m.norm1());

  auto const result = view * std::vector<int>{1, 2};
  EXPECT_EQ(result, (std::vector<int>{9, 30, 15}));
}

TEST(MyMat0Test, ConstTransposeViewIsReadable)
{
  // A transpose view built from a const matrix must still support read-only
  // access.
  auto const m = makeReferenceMatrix<ROWMAJOR>();
  TransposeView<MyMat0<double, ROWMAJOR> const> view{m};
  EXPECT_DOUBLE_EQ(view(2, 1), -6.0);
}
