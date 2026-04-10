/*!
 * @file SaddlePointMat.hpp
 * @brief Lightweight block representation of the saddle-point matrix used by
 *        the Darcy example.
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_
#include "BasicType.hpp"
#include "SparseBlockMatrix.hpp"
#include <Eigen/Sparse>
#include <iostream>
#include <stdexcept>
namespace FVCode3D
{
//! Block wrapper for the sparse saddle-point operator.
/*!
 * @class SaddlePointMat
 * The class stores the matrix through its natural blocks M, B and T and
 * delegates the actual sparse storage to `apsc::SparseBlockMatrix`.
 *
 * This design is useful in two ways:
 * - iterative solvers can apply matrix-vector products without assembling
 *   auxiliary dense objects;
 * - preconditioners can reuse the same blocks directly when forming Schur
 *   complement approximations.
 *
 * The matrix is stored in the symmetric-indefinite form
 * \f[
 * \begin{bmatrix}
 * M & B^T\\
 * B & T
 * \end{bmatrix}
 * \f]
 * where typically \f$T=-K\f$ and \f$K\f$ is symmetric positive semidefinite.
 *
 * The current implementation only supports this symmetric layout.
 */
class SaddlePointMat
{
public:
  using Scalar = double;
  //! @name Constructor & Destructor
  //@{
  //! Empty-Constructor
  SaddlePointMat() = default;

  //! Construct the 2x2 saddle-point operator from its sparse blocks.
  /*!
   * @tparam SMAT anything convertible to a SpMat
   * @param Mmat M matrix
   * @param Bmat B matrix
   * @param Tmat T matrix
   * @param isSymUndef Must be true in the current implementation.
   */
  template <typename SMAT>
  SaddlePointMat(SMAT &&Mmat, SMAT &&Bmat, SMAT &&Tmat, bool isSymUndef = true)
    : isSymUndef{isSymUndef}, matrixData{{Mmat.rows(), Bmat.rows(), 0},
                                         {Mmat.cols(), Bmat.rows(), 0}}
  {
    if(!isSymUndef)
      throw std::runtime_error(
        "This version supports only symmetric saddle point matrices");
    matrixData.setBlock({0, 0}, std::forward<SMAT>(Mmat));
    matrixData.setBlock({1, 0}, std::forward<SMAT>(Bmat));
    matrixData.setBlock({1, 1}, std::forward<SMAT>(Tmat));
    matrixData.addTranspose({0, 1}, {1, 0});
  }
  //! Construct an empty saddle-point operator with the requested block sizes.
  /*!
   * @param Mdim M block dimension
   * @param Brow B block row
   * @param isSymUndef Must be true in the current implementation.
   */
  SaddlePointMat(const UInt Mdim, const UInt Brow, bool isSymUndef = true)
    : isSymUndef(isSymUndef), matrixData{
                                {Eigen::Index(Mdim), Eigen::Index(Brow), 0},
                                {Eigen::Index(Mdim), Eigen::Index(Brow), 0}}
  {
    if(!isSymUndef)
      throw std::runtime_error(
        "This version supports only symmetric saddle point matrices");
  }
  //@}

  Eigen::Index
  rows() const
  {
    return matrixData.rows();
  }
  Eigen::Index
  cols() const
  {
    return matrixData.cols();
  }

  //! @name Methods
  //!@{
  //! Replace the stored blocks with new sparse matrices.
  /*!
   * @tparam SMAT anything convertible to a SpMat
   * @param Mmat M matrix
   * @param Bmat B matrix
   * @param Tmat T matrix
   */
  template <typename SMAT>
  void
  Set(SMAT &&Mmat, SMAT &&Bmat, SMAT &&Tmat)
  {
    matrixData.setBlock({0, 0}, std::forward<SMAT>(Mmat));
    matrixData.setBlock({1, 0}, std::forward<SMAT>(Bmat));
    matrixData.setBlock({1, 1}, std::forward<SMAT>(Tmat));
    matrixData.addTranspose({0, 1}, {1, 0});
  }

  //! Set the matrix layout flag.
  /*!
   * Only the symmetric-indefinite form is supported, so passing `false`
   * results in an exception.
   *
   * @param coeff Non-zero for symmetric-indefinite form.
   */
  void
  Set_isSymUndef(const int coeff)
  {
    isSymUndef = coeff;
    if(!isSymUndef)
      throw std::runtime_error(
        "This version supports only symmetric saddle point matrices");
  }

  //! Compress all internal sparse blocks.
  /*!
   * Compress the block matrices M, B and T
   */
  void
  makeCompressed()
  {
    matrixData.makeCompressed();
  };
  //!@}

  //! @name Get Methods
  //!@{
  //! Get M block (read only)
  /*!
   * @return A const reference to the M block
   */
  const SpMat &
  getM() const
  {
    return matrixData.getBlock({0, 0});
  }

  //! Get M block
  /*!
   * @return A reference to the M block
   */
  SpMat &
  getM()
  {
    return matrixData.getBlock({0, 0});
  }

  //! Get B block (read only)
  /*!
   * @return A const reference to the B block
   */
  const SpMat &
  getB() const
  {
    return matrixData.getBlock({1, 0});
  }

  //! Get B block
  /*!
   * @return A reference to the B block
   */
  SpMat &
  getB()
  {
    return matrixData.getBlock({1, 0});
  }

  //! Get T block (read only)
  /*!
   * @return A const reference to the T block
   */
  const SpMat &
  getT() const
  {
    return matrixData.getBlock({1, 1});
  }

  //! Get T block
  /*!
   * @return A reference to the T block
   */
  SpMat &
  getT()
  {
    return matrixData.getBlock({1, 1});
  }
  //!@}

  //! @name Methods
  //!@{
  //! Resize the block structure without populating the coefficients.
  /*!
   *
   * @param Mdim number or wows
   * @param Brow number of columns
   */
  void
  resize(const UInt Mdim, const UInt Brow)
  {
    matrixData.resize({Eigen::Index(Mdim), Eigen::Index(Brow)},
                      {Eigen::Index(Mdim), Eigen::Index(Brow)});
  }
  //! Frobenius norm squared of the assembled block matrix.
  Real
  squaredNorm() const
  {
    return matrixData.squaredNorm();
  }
  //! Frobenius norm of the assembled block matrix.
  Real
  norm() const
  {
    return std::sqrt(this->squaredNorm());
  }
  //! Number of non-zero coefficients across all stored blocks.
  UInt
  nonZeros() const
  {
    return matrixData.nonZeros();
  }
  //!@}

  //! @name Operators
  //!@{
  //! Apply the block matrix to a vector.
  /*!
   * @param x Input vector.
   * @return Product between the saddle-point operator and `x`.
   */
  Vector
  operator*(const Vector &x) const
  {
    return matrixData * x;
  }
  //! Clear all blocks and release their storage.
  void
  clear()
  {
    matrixData.clear();
  }
  //! Assemble and return the full sparse matrix.
  SpMat
  fullMatrix() const
  {
    return matrixData.fullMatrix();
  };
  //!@}

  //! Flag describing the selected block layout.
  bool isSymUndef = true;
  /*!
   * Split the lower-right block into cell and fracture sub-blocks.
   *
   * After this transformation the internal representation becomes a 3x3 block
   * operator used by the `DoubleSaddlePoint*` preconditioners.
   */
  void convertToDoubleSaddlePoint();

  //! Return the internal block matrix representation.

  apsc::SparseBlockMatrix<double, 3, 3> const &
  sparseBlockMatrix() const
  {
    return matrixData;
  }

private:
  apsc::SparseBlockMatrix<double, 3, 3> matrixData;
};

inline void
SaddlePointMat::convertToDoubleSaddlePoint()
{
  // The code assumes that fracture unknowns correspond to the non-empty part of
  // the trailing block T. Those rows/columns are separated to expose the 3x3
  // structure required by the double saddle-point preconditioners.
  auto        nVel = matrixData.cols({0, 0});
  auto const &T = matrixData.getBlock({1, 1});
  UInt        nFrac = 0;
  for(int k = 0; k < T.outerSize(); ++k)
    {
      for(SpMat::InnerIterator it(T, k); it; ++it)
        {
          if(it.value() != 0.0)
            {
              ++nFrac;
              break;
            }
        }
    }
  if(T.rows() < Eigen::Index(nFrac))
    throw std::runtime_error("Something strange: matrix T inconsistent");
  UInt nCell = T.rows() - nFrac;
  std::swap(matrixData.getBlock({2, 2}), matrixData.getBlock({1, 1}));
  SpMat &Ttilde = matrixData.getBlock({2, 2});
  Ttilde = Ttilde.bottomRightCorner(nFrac, nFrac);
  matrixData.getBlock({2, 0}) =
    matrixData.getBlock({1, 0}).bottomLeftCorner(nFrac, nVel);
  matrixData.getBlock({1, 0}) =
    matrixData.getBlock({1, 0}).topLeftCorner(nCell, nVel);
  matrixData.addTranspose({0, 2}, {2, 0});
  matrixData.getBlock({1, 1}).resize(nCell, nCell);
  matrixData.getBlock({1, 2}).resize(nCell, nFrac);
  matrixData.getBlock({2, 1}).resize(nFrac, nCell);
  matrixData.changeOffsets({nVel, Eigen::Index(nCell), Eigen::Index(nFrac)},
                           {nVel, Eigen::Index(nCell), Eigen::Index(nFrac)});
  /*
      for (auto i=0;i<3;++i)
        for (auto j=0;j<3;++j)
          {
            std::clog<<"Block ("<<i<<","<<j<<")\n";
            std::clog<<"Rows declared"<<matrixData.rows({i,j})<<"Effective
     "<<matrixData.getBlock({i,j}).rows()<<std::endl;; std::clog<<"Cols
     declared"<<matrixData.cols({i,j})<<"Effective
     "<<matrixData.getBlock({i,j}).cols()<<std::endl;

          }
  */
}

} // namespace FVCode3D

#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_ \
        */
