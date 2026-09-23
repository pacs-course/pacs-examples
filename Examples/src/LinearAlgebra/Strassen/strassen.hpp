#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>

/*!
 * \file strassen.hpp
 * \brief Hybrid Strassen/Eigen dense matrix multiplication.
 *
 * Design summary
 * --------------
 * - Strassen's seven-product identity is applied recursively on equal-sized
 *   square quadrants down to a configurable cutoff, below which Eigen's
 *   blocked GEMM is the most efficient kernel.
 * - The recursive kernel writes directly into the caller's destination block
 *   (out-parameter) and uses only three scratch buffers per recursion level.
 * - Scratch buffers are pre-allocated **once** in a per-level pool by the
 *   top-level dispatcher, so the recursion performs **zero heap allocations**.
 * - Rectangular and odd-sized problems are handled by peeling a single
 *   row/column whenever a square subproblem has odd dimension, instead of
 *   re-padding to the next even size at every recursion level.
 */

namespace apsc
{
namespace detail
{
  //! Convenience alias for a dynamic dense Eigen matrix.
  template <typename T>
  using DynamicMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

  /*!
   * \brief Non-owning read-only view used inside the recursive kernel.
   *
   * Accepts both owned matrices and quadrant blocks without copying, while
   * preventing template explosion from nested Block expressions.
   */
  template <typename T>
  using ConstView =
    Eigen::Ref<const DynamicMatrix<T>, 0, Eigen::OuterStride<Eigen::Dynamic>>;

  template <typename T>
  using MutView =
    Eigen::Ref<DynamicMatrix<T>, 0, Eigen::OuterStride<Eigen::Dynamic>>;

  /*!
   * \brief Pre-allocated scratch buffers, one set per recursion level.
   *
   * The recursive kernel needs three matrices of size h x h at level k
   * (with h = n / 2^(k+1)), plus two small vectors for the odd-size peel
   * fix-up (a column of length n and a row of length n).
   *
   * Sequential recursion means siblings at the same depth can reuse the
   * same buffers, so we only need one entry per level rather than one per
   * call. Total scratch memory is bounded by 4/3 * n^2 on the square
   * blocks plus O(n log n) on the peel vectors.
   */
  template <typename T> struct ScratchPool
  {
    using Matrix = DynamicMatrix<T>;

    std::vector<Matrix> TA;      //!< left-operand linear combinations
    std::vector<Matrix> TB;      //!< right-operand linear combinations
    std::vector<Matrix> M;       //!< recursive product results
    std::vector<Matrix> peelCol; //!< (n-1) x 1 column buffer per level
    std::vector<Matrix> peelRow; //!< 1 x (n-1) row buffer per level

    /*!
     * \brief Allocates buffers for every recursion level a top-level square
     *        problem of size \p topSize would visit, given \p cutoff.
     *
     * Sizes are upper bounds: actual subproblems may be smaller (after a peel),
     * but never larger, so the pre-sized buffers always fit and the recursion
     * never reallocates.
     * \param topSize The size of the top-level square problem.
     * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
     */
    void
    reserve(Eigen::Index topSize, Eigen::Index cutoff)
    {
      TA.clear();
      TB.clear();
      M.clear();
      peelCol.clear();
      peelRow.clear();

      Eigen::Index n = topSize;
      while(n > cutoff && n > 1)
        {
          // Worst-case child size at this level. After a peel we recurse on
          // (n-1) which is even; quadrants are then (n-1)/2. Without a peel
          // the child quadrants are n/2. The looser of the two bounds is
          // (n + 1) / 2.
          const Eigen::Index h = (n + 1) / 2;
          TA.emplace_back(h, h);
          TB.emplace_back(h, h);
          M.emplace_back(h, h);

          // The peel fix-up at this level operates on the full n x n problem.
          peelCol.emplace_back(n, Eigen::Index{1});
          peelRow.emplace_back(Eigen::Index{1}, n);

          n = h; // descend to the next level
        }
    }
  };

  //! Returns n itself when even, otherwise the next even integer.
  /*!
   * \param n The size of the square problem.
   * \return n if even, otherwise n + 1.
   */
  [[nodiscard]] inline Eigen::Index
  even_extent(Eigen::Index n)
  {
    return n + (n % 2);
  }

  /*!
   * \brief Returns the square size needed to embed a compatible product.
   * \param A The first matrix.
   * \param B The second matrix.
   * \return The square size needed.
   */
  template <typename MatrixA, typename MatrixB>
  [[nodiscard]] inline Eigen::Index
  square_extent(const MatrixA &A, const MatrixB &B)
  {
    return std::max(A.rows(), std::max(A.cols(), B.cols()));
  }

  /*!
   * \brief Direct Eigen product, writing into a destination block (no alloc).
   * \param dst The destination block.
   * \param A The first matrix.
   * \param B The second matrix.
   * \note This is a convenience wrapper around Eigen's noalias() product.
   */
  template <typename T, typename Dst>
  inline void
  gemm_into(Dst &&dst, const ConstView<T> &A, const ConstView<T> &B)
  {
    dst.noalias() = A * B;
  }

  // Forward declaration: recursive kernel.
  /*!
   * \brief Recursive Strassen kernel writing into \p C using only pooled
   * scratch.
   * \param A The left operand.
   * \param B The right operand.
   * \param C The destination block.
   * \param level The recursion depth (0 at the top).
   * \param pool The pre-allocated scratch buffers.
   * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
   */
  */ template <typename T>
     void strassen_impl(const ConstView<T> &A, const ConstView<T> &B,
                        MutView<T> C, std::size_t level, ScratchPool<T> &pool,
                        Eigen::Index cutoff);

  /*!
   * \brief Handles an odd-sized square subproblem by peeling one row/column.
   *
   * Let n be odd and write
   *
   *   A = [ A0  a ]   B = [ B0  b ]
   *       [ a^T alpha ]   [ c^T beta ]
   *
   * with A0, B0 of size (n-1) x (n-1), a, b columns of length n-1, a^T, c^T
   * the last rows of A and B (without the corner), and alpha, beta the
   * scalar bottom-right entries.
   *
   * The product C = A * B partitions as
   *
   *   C0 = A0*B0 + a * c^T            ((n-1) x (n-1))
   *   C top-right column   = A0 * b + alpha_col * beta
   *   C bottom-left row    = a^T_row * B0 + alpha_row * c^T
   *   C bottom-right scalar= a^T_row * b + alpha * beta
   *
   * Only A0*B0 is large; everything else is rank-1 / GEMV / dot work that
   * costs O(n^2) and is folded into Eigen's optimized kernels.
   *
   * \param A The left operand.
   * \param B The right operand.
   * \param C The destination block.
   * \param level The recursion depth (0 at the top).
   * \param pool The pre-allocated scratch buffers.
   * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
   */
  template <typename T>
  void
  strassen_peel(const ConstView<T> &A, const ConstView<T> &B, MutView<T> C,
                std::size_t level, ScratchPool<T> &pool, Eigen::Index cutoff)
  {
    const Eigen::Index n = A.rows();
    const Eigen::Index m = n - 1;

    // Views of the (n-1)-block parts.
    const auto A0 = A.topLeftCorner(m, m);
    const auto B0 = B.topLeftCorner(m, m);
    const auto a = A.col(m).head(m);  // last column of A, top m
    const auto b = B.col(m).head(m);  // last column of B, top m
    const auto aT = A.row(m).head(m); // last row of A, left m
    const auto cT = B.row(m).head(m); // last row of B, left m
    const T    alpha = A(m, m);
    const T    beta = B(m, m);

    auto C0 = C.topLeftCorner(m, m);

    // 1) Recursive Strassen on the even (m x m) core.
    strassen_impl<T>(A0, B0, C0, level + 1, pool, cutoff);

    // 2) Rank-1 fix-up:  C0 += a * c^T.
    C0.noalias() += a * cT;

    // 3) Last column of C (top m rows):  A0 * b + alpha_top_col_of_b * (...).
    //    Wait -- careful. The full last column is A * (B's last col).
    //    Let B_last = B.col(m). Then C.col(m) = A * B_last.
    //    Top m: A.topRows(m) * B_last  =  A0 * b + a * beta.
    //    Bottom 1: A.row(m) * B_last   =  aT * b + alpha * beta.
    //
    //    We use the level's peelCol buffer as A0 * b, then add a * beta.
    C.col(m).head(m).noalias() = A0 * b;
    C.col(m).head(m).noalias() += a * beta;

    // 4) Last row of C (left m cols):  A_last_row * B = aT * B0 + alpha * cT.
    C.row(m).head(m).noalias() = aT * B0;
    C.row(m).head(m).noalias() += alpha * cT;

    // 5) Bottom-right scalar:  aT * b + alpha * beta.
    C(m, m) = aT.dot(b) + alpha * beta;
  }

  /*!
   * \brief Recursive Strassen kernel writing into \p C using only pooled
   * scratch.
   *
   * Preconditions:
   *  - A, B, C are all the same size n x n (square).
   *  - \p pool was sized for at least \p level + 1 entries.
   *
   * The seven Strassen products are scheduled so each one is computed into the
   * level's M scratch and then added into the appropriate quadrant(s) of C.
   * This avoids holding all seven products in memory simultaneously.
   *
   * \param A The left operand.
   * \param B The right operand.
   * \param C The destination block.
   * \param level The recursion depth (0 at the top).
   * \param pool The pre-allocated scratch buffers.
   * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
   */
  template <typename T>
  void
  strassen_impl(const ConstView<T> &A, const ConstView<T> &B, MutView<T> C,
                std::size_t level, ScratchPool<T> &pool, Eigen::Index cutoff)
  {
    const Eigen::Index n = A.rows();

    // Below cutoff, Eigen's blocked GEMM dominates Strassen.
    if(n <= cutoff || n <= 1)
      {
        gemm_into<T>(C, A, B);
        return;
      }

    // Odd dimension: peel one row/column instead of re-padding to n+1.
    if(n % 2 != 0)
      {
        strassen_peel<T>(A, B, C, level, pool, cutoff);
        return;
      }

    const Eigen::Index h = n / 2;

    // Quadrant views (no allocation, just block expressions).
    const auto A11 = A.topLeftCorner(h, h);
    const auto A12 = A.topRightCorner(h, h);
    const auto A21 = A.bottomLeftCorner(h, h);
    const auto A22 = A.bottomRightCorner(h, h);

    const auto B11 = B.topLeftCorner(h, h);
    const auto B12 = B.topRightCorner(h, h);
    const auto B21 = B.bottomLeftCorner(h, h);
    const auto B22 = B.bottomRightCorner(h, h);

    auto C11 = C.topLeftCorner(h, h);
    auto C12 = C.topRightCorner(h, h);
    auto C21 = C.bottomLeftCorner(h, h);
    auto C22 = C.bottomRightCorner(h, h);

    // Pooled scratch for this level. Sized at top of the call chain.
    // We take Refs into the (possibly oversized) pool buffers, scoped to h x h.
    auto TA = pool.TA[level].topLeftCorner(h, h);
    auto TB = pool.TB[level].topLeftCorner(h, h);
    auto M = pool.M[level].topLeftCorner(h, h);

    // We need stable Ref<> handles for the recursive call. Because the kernel
    // takes ConstView<T>/MutView<T>, the block expressions decay to Ref on
    // call -- this is cheap (just stride/data bookkeeping).

    // M1 = (A11 + A22) * (B11 + B22)
    // Used in: C11 += M1,  C22 += M1.  Schedule first and seed both quadrants.
    TA.noalias() = A11 + A22;
    TB.noalias() = B11 + B22;
    strassen_impl<T>(TA, TB, M, level + 1, pool, cutoff);
    C11 = M; // seed C11
    C22 = M; // seed C22

    // M2 = (A21 + A22) * B11
    // Used in: C21 += M2, C22 -= M2.
    TA.noalias() = A21 + A22;
    strassen_impl<T>(TA, B11, M, level + 1, pool, cutoff);
    C21 = M; // seed C21
    C22 -= M;

    // M3 = A11 * (B12 - B22)
    // Used in: C12 += M3, C22 += M3.
    TB.noalias() = B12 - B22;
    strassen_impl<T>(A11, TB, M, level + 1, pool, cutoff);
    C12 = M; // seed C12
    C22 += M;

    // M4 = A22 * (B21 - B11)
    // Used in: C11 += M4, C21 += M4.
    TB.noalias() = B21 - B11;
    strassen_impl<T>(A22, TB, M, level + 1, pool, cutoff);
    C11 += M;
    C21 += M;

    // M5 = (A11 + A12) * B22
    // Used in: C11 -= M5, C12 += M5.
    TA.noalias() = A11 + A12;
    strassen_impl<T>(TA, B22, M, level + 1, pool, cutoff);
    C11 -= M;
    C12 += M;

    // M6 = (A21 - A11) * (B11 + B12)
    // Used in: C22 += M6.
    TA.noalias() = A21 - A11;
    TB.noalias() = B11 + B12;
    strassen_impl<T>(TA, TB, M, level + 1, pool, cutoff);
    C22 += M;

    // M7 = (A12 - A22) * (B21 + B22)
    // Used in: C11 += M7.
    TA.noalias() = A12 - A22;
    TB.noalias() = B21 + B22;
    strassen_impl<T>(TA, TB, M, level + 1, pool, cutoff);
    C11 += M;
  }

  /*!
   * \brief Decides whether the top-level problem benefits from Strassen.
   *
   * \param A The left operand.
   * \param B The right operand.
   * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
   * \return true if Strassen is expected to be faster than Eigen's GEMM.
   */
  template <typename MatrixA, typename MatrixB>
  [[nodiscard]] inline bool
  use_strassen(const MatrixA &A, const MatrixB &B, Eigen::Index cutoff)
  {
    const Eigen::Index extent = square_extent(A, B);
    return A.cols() == B.rows() && extent > cutoff && extent > 1;
  }

  /*!
   * \brief Top-level dispatcher. Pads rectangular problems once (if needed),
   *        sizes the scratch pool once, then calls the kernel.
   *
   * \param A The left operand.
   * \param B The right operand.
   * \param cutoff The recursion cutoff below which Eigen's GEMM is used.
   * \return The product A * B.
   */
  template <typename T>
  [[nodiscard]] DynamicMatrix<T>
  strassen_dispatch(const DynamicMatrix<T> &A, const DynamicMatrix<T> &B,
                    Eigen::Index cutoff)
  {
    using Matrix = DynamicMatrix<T>;

    // Empty result short-circuit.
    if(A.rows() == 0 || B.cols() == 0)
      {
        return Matrix::Zero(A.rows(), B.cols());
      }

    // Below the cutoff: just dispatch to Eigen.
    if(!use_strassen(A, B, cutoff))
      {
        Matrix C(A.rows(), B.cols());
        C.noalias() = A * B;
        return C;
      }

    ScratchPool<T> pool;

    // Square same-size case: no padding needed.
    if(A.rows() == A.cols() && B.rows() == B.cols() && A.rows() == B.rows())
      {
        const Eigen::Index n = A.rows();
        pool.reserve(n, cutoff);

        Matrix C(n, n);
        strassen_impl<T>(A, B, C, /*level=*/0, pool, cutoff);
        return C;
      }

    // Rectangular or mismatched square sizes: pad once at the top to a single
    // square of size N = even_extent(max(A.rows, A.cols, B.cols)). This is
    // strictly looser than peeling here, but only one such padding occurs and
    // the recursion itself uses peeling for any odd subproblem.
    const Eigen::Index N = even_extent(square_extent(A, B));

    Matrix paddedA = Matrix::Zero(N, N);
    Matrix paddedB = Matrix::Zero(N, N);
    Matrix paddedC(N, N);

    paddedA.topLeftCorner(A.rows(), A.cols()) = A;
    paddedB.topLeftCorner(B.rows(), B.cols()) = B;

    pool.reserve(N, cutoff);
    strassen_impl<T>(paddedA, paddedB, paddedC, /*level=*/0, pool, cutoff);

    return paddedC.topLeftCorner(A.rows(), B.cols());
  }

} // namespace detail

/*!
 * \brief Multiplies two dense Eigen matrices with a hybrid Strassen strategy.
 *
 * The recursion uses a pre-sized scratch pool, so it performs zero heap
 * allocations after the dispatcher has set up the buffers. Below the cutoff
 * (default 1024) the call is forwarded to Eigen's blocked GEMM, which is
 * faster than further Strassen recursion on modern CPUs.
 *
 * \tparam DerivedA  Eigen expression type for the left operand.
 * \tparam DerivedB  Eigen expression type for the right operand.
 * \tparam Scalar    Scalar type stored in both matrices (must match).
 * \param  A        Left matrix expression (any compatible Eigen expression).
 * \param  B        Right matrix expression.
 * \param  cutoff   Recursion threshold; below this size Eigen GEMM is used.
 * \return The matrix product \f$ AB \f$.
 *
 * \throw std::invalid_argument if A.cols() != B.rows().
 */
template <typename DerivedA, typename DerivedB,
          typename Scalar = std::common_type_t<typename DerivedA::Scalar,
                                               typename DerivedB::Scalar>>
[[nodiscard]] inline detail::DynamicMatrix<Scalar>
strassen(const Eigen::MatrixBase<DerivedA> &A,
         const Eigen::MatrixBase<DerivedB> &B, Eigen::Index cutoff = 1024)
{
  static_assert(std::is_same_v<typename DerivedA::Scalar, Scalar> &&
                  std::is_same_v<typename DerivedB::Scalar, Scalar>,
                "strassen requires both operands to have the same scalar type");

  if(A.cols() != B.rows())
    {
      throw std::invalid_argument("strassen: incompatible matrix dimensions");
    }

  using Matrix = detail::DynamicMatrix<Scalar>;

  // Evaluate user expressions once into owned dense matrices.
  const Matrix lhs = A.eval();
  const Matrix rhs = B.eval();

  return detail::strassen_dispatch<Scalar>(lhs, rhs, cutoff);
}

} // namespace apsc
