/*!
 * @file SparseBlock.hpp
 * @brief Extract Sparse Block for SpMat
 */
#ifndef FVCODE3D_SPARSEBLOCK_HPP_
#define FVCODE3D_SPARSEBLOCK_HPP_

#include <Eigen/Sparse>
#include "BasicType.hpp"

namespace FVCode3D
{

//! Extract a block from a SpMat
/*!
 * Only for ColMajor Sparse Matrix
 */
SpMat sparseBlock(SpMat _matrix, UInt _ibegin, UInt _jbegin, UInt _icount, UInt _jcount);

//! Extract the top left block of a Sparse Matrix
SpMat sparseTopLeftBlock(SpMat _matrix, UInt _icount, UInt _jcount);

//! Extract the top right block of a Sparse Matrix
SpMat sparseTopRightBlock(SpMat _matrix, UInt _icount, UInt _jcount);

//! Extract the bottom left block of a Sparse Matrix
SpMat sparseBottomLeftBlock(SpMat _matrix, UInt _icount, UInt _jcount);

//! Extract the bottom right block of a Sparse Matrix
SpMat sparseBottomRightBlock(SpMat _matrix, UInt _icount, UInt _jcount);

} // namespace Eigen

#endif // FVCODE3D_SPARSEBLOCK_HPP_
