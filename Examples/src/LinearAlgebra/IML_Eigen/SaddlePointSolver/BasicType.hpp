/*!
 * @file basic_type.hpp
 * @brief Basic definition types
 */

#ifndef FVCODE3D_BASIC_TYPE_HPP_
#define FVCODE3D_BASIC_TYPE_HPP_

#include <stdint.h>

#include <Eigen/Sparse>

#define FVCODE3D_HAS_UMFPACK
//#define FVCODE3D_HAS_SAMG

namespace FVCode3D
{

  //! Type for real numbers
  typedef double Real;
  
  //! Type for unsigned short int
  typedef uint8_t SUInt;
  
  //! Type for integer numbers
  typedef uint64_t UInt;
  
  //! Generic integer data
  typedef int64_t Int;
  
  //! bit-flag with up to 8 different flags
  typedef uint8_t Flag8bit;
  
  //! bit-flag with up to 16 different flags
  typedef uint16_t Flag16bit;
  
  //! bit-flag with up to 32 different flags
  typedef uint32_t Flag32bit;
  
  //! bit-flag with up to 64 different flags
  typedef uint64_t Flag64bit;
  
  //! PI
  constexpr Real _PI_ = 3.14159265358979323846;
  
  //! Type for eigen column-major sparse matrix of Real
  typedef Eigen::SparseMatrix<Real> SpMat;
  
  //! Type for eigen row-major sparse matrix of Real
  typedef Eigen::SparseMatrix<Real, Eigen::RowMajor> SpMatRM;
  
  //! The diagonal matrix representing the lumped inner product matrix
  typedef Eigen::DiagonalMatrix<Real, Eigen::Dynamic> DiagMat;

  //! Type for eigen vectors
  typedef Eigen::VectorXd Vector;

  //! Type for eigen dense matrix of real with dynamic size
  typedef Eigen::Matrix<Real,Eigen::Dynamic,Eigen::Dynamic> Mat;

  //! Type for eigen dense matrix of real with dynamic row size and 3 fixed column size
  typedef Eigen::Matrix<Real,Eigen::Dynamic,3> Mat3;

  //! Type for eigen dense matrix of real with 3x3 size
  typedef Eigen::Matrix<Real,3,3> Mat33;

  //! Type for eigen UInt vectors
  typedef Eigen::Matrix<UInt, Eigen::Dynamic, 1> UIntVector;

  //! Type for eigen triplets
  typedef Eigen::Triplet<Real> Triplet;

  enum PrecondSwitch {Diagonal, BlockDiagonal, BlockTriangular, ILU, HSS, Identity, DoubleSaddlePoint, DoubleSaddlePointSym};

} // namespace FVCode3D

#endif /* FVCODE3D_BASIC_TYPE_HPP_ */
