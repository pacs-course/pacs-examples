/*
 * SaddlePointMat.hpp
 *
 *  Created on: Jul 9, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_
#include "BasicType.hpp"
#include "SparseBlockMatrix.hpp"
#include <Eigen/Sparse>
#include <stdexcept>
#include <iostream>
namespace FVCode3D
{
  //! Class for assembling a saddle point matrix.
  /*!
   * @class SaddlePointMat
   * This class implements a generic saddle point matrix. It consists of 3 block matrices
   * M, B and T. It will be build up through an object of type SaddlePoint_StiffMatrix
   * that is the assembler of the numerical method and it overloads the *(Vector) operator
   * to use the blocks matrix to compute the matrix-vector product. In this way we avoid
   * to store the whole system matrix and we use the same 3 blocks (without any copy of them)
   * to make everything we need: matrix-vector product, building and inverting the preconditioner.
   * Clearly, this class is interesting only with an iterative system solving the system.
   * Constructors, assignement, destructor and so on are all defaulted.
   * The matrix will always contain the blocks in this form
   * \f[
   * \begin{bmatrix}
   * M & B^T\\
   * B & T
   * \end{bmatrix}
   * \f]
   *  being \f$T=-K\f$ and \f$K\f$ a symmetric non-negative definite matrix.
   * However, if isSymUndef is set to false, it operates like the un-symmetric matrix
   * \f[
   * \begin{bmatrix}
   * M & B^T\\
   * -B & -T
   * \end{bmatrix}=
   * \begin{bmatrix}
   * M & B^T\\
   * -B & K
   * \end{bmatrix}=
   * \f]
  */
  class SaddlePointMat
  {
  public:
      using Scalar=double;
      //! @name Constructor & Destructor
      //@{
      //! Empty-Constructor
      SaddlePointMat()=default;

      //! Construct a saddle point matrix
      /*!
       * @tparam SMAT anything convertible to a SpMat
       * @param Mmat M matrix
       * @param Bmat B matrix
       * @param Tmat T matrix
       * @param isSymUndef true: symmetric form
       */
      template <typename SMAT>
      SaddlePointMat(SMAT&& Mmat, SMAT&& Bmat, SMAT&& Tmat,bool isSymUndef=true):
        isSymUndef{isSymUndef},matrixData{{Mmat.rows(),Bmat.rows(),0},{Mmat.cols(),Bmat.rows(),0}}
      {
        if (!isSymUndef)
          throw std::runtime_error("This version supports only symmetric saddle point matrices");
        matrixData.setBlock({0,0},std::forward<SMAT>(Mmat));
        matrixData.setBlock({1,0},std::forward<SMAT>(Bmat));
        matrixData.setBlock({1,1},std::forward<SMAT>(Tmat));
        matrixData.addTranspose({0,1},{1,0});
      }
    //! Construct a saddle point matrix
      /*!
       * @param Mdim M block dimension
       * @param Brow B block row
       * @param Bcol B block col
       */
     SaddlePointMat(const UInt Mdim,const UInt Brow, bool isSymUndef=true):
                  isSymUndef(isSymUndef), matrixData{{Eigen::Index(Mdim),Eigen::Index(Brow),0},{Eigen::Index(Mdim),Eigen::Index(Brow),0}}
     {
       if (!isSymUndef)
                 throw std::runtime_error("This version supports only symmetric saddle point matrices");
     }
     //@}

     Eigen::Index rows()const {return matrixData.rows();}
     Eigen::Index cols()const {return matrixData.cols();}


      //! @name Methods
      //!@{
      //! Sets the saddle point matrix
      /*!
       * @tparam SMAT anything convertible to a SpMat
       * @param Mmat M matrix
       * @param Bmat B matrix
       * @param Tmat T matrix
       */
     template <typename SMAT>
      void Set(SMAT&& Mmat, SMAT&& Bmat, SMAT&& Tmat)
          {
       matrixData.setBlock({0,0},std::forward<SMAT>(Mmat));
       matrixData.setBlock({1,0},std::forward<SMAT>(Bmat));
       matrixData.setBlock({1,1},std::forward<SMAT>(Tmat));
       matrixData.addTranspose({0,1},{1,0});
          }

          //! Set the flag isSymUndef
      /*!
       * @param coeff The flag to be set 1=is  Sym Undef -1 is UnSym pdef
       */
     void Set_isSymUndef(const int coeff)
     {
       isSymUndef = coeff;
       if (!isSymUndef)
         throw std::runtime_error("This version supports only symmetric saddle point matrices");
     }

          //! Compress the block matrices
      /*!
       * Compress the block matrices M, B and T
       */
      void makeCompressed()
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
      const SpMat & getM() const
          {return matrixData.getBlock({0,0});
          }

      //! Get M block
      /*!
       * @return A reference to the M block
       */
      SpMat & getM()
          {return matrixData.getBlock({0,0});
          }

      //! Get B block (read only)
      /*!
       * @return A const reference to the B block
       */
      const SpMat & getB() const
          {return matrixData.getBlock({1,0});}

      //! Get B block
      /*!
       * @return A reference to the B block
       */
      SpMat & getB()
          {return matrixData.getBlock({1,0});}

      //! Get T block (read only)
      /*!
       * @return A const reference to the T block
       */
      const SpMat & getT() const
          {return matrixData.getBlock({1,1});}

      //! Get T block
      /*!
       * @return A reference to the T block
       */
      SpMat & getT()
          {return matrixData.getBlock({1,1});}
      //!@}

     //! @name Methods
     //!@{
          //! Resize the system
     /*!
      *
      * @param Mdim number or wows
      * @param Brow number of columns
      */
      void resize(const UInt Mdim,const UInt Brow)
          {
        matrixData.resize({Eigen::Index(Mdim),Eigen::Index(Brow)},{Eigen::Index(Mdim),Eigen::Index(Brow)});
          }
      /*!
       * Frobenius norm of the matrix sqared
       *
       * @return the norm squared
       */
      Real squaredNorm()const
      {
        return matrixData.squaredNorm();
      }
      /*!
        * Frobenius norm of the matrix
        *
        * @return the norm
        */
      Real norm()const {return std::sqrt(this->squaredNorm());}
          //! Get the number of non zero
      /*!
       * @return the number of non zero
       */
      UInt nonZeros() const { return matrixData.nonZeros(); }
      //!@}

      //! @name Operators
      //!@{
          //! Overload of matrix-vector product operator using the blocks
      /*!
       * @param x A const reference to the vector
       * @return The Vector resulting from the matrix-vector product
       */
      Vector operator * (const Vector & x) const
          {
        return matrixData * x;
          }
      //! Clears matrix and frees memory
      void clear(){matrixData.clear();}
      /*!
       * @return the full matrix
       */
       SpMat fullMatrix() const
       {return matrixData.fullMatrix();};
      //!@}

      //! A flag indicating if it is sym-undef or defpos-unsym
         bool  isSymUndef=true;
/*!
 * Converts the matrix to a double saddle point structire
 */
         void convertToDoubleSaddlePoint();

         //! Returns the contained block matrix as constant reference

         apsc::SparseBlockMatrix<double,3,3> const & sparseBlockMatrix() const
         {
           return matrixData;
         }

  private:
         apsc::SparseBlockMatrix<double,3,3> matrixData;
  };

  inline void SaddlePointMat::convertToDoubleSaddlePoint()
  {
    auto nVel = matrixData.cols({0,0});
    auto const & T=matrixData.getBlock({1,1});
    UInt nFrac=0;
    for (int k =0; k< T.outerSize();++k)
    {
        for (SpMat::InnerIterator it(T,k); it; ++it)
          {
            if (it.value() != 0.0)
              {
                ++nFrac;
                break;
              }
          }
    }
    if (T.rows()<Eigen::Index(nFrac)) throw std::runtime_error("Something strange: matrix T inconsistent");
    UInt nCell= T.rows()-nFrac;
    std::swap(matrixData.getBlock({2,2}),matrixData.getBlock({1,1}));
    SpMat & Ttilde=matrixData.getBlock({2,2});
    Ttilde=Ttilde.bottomRightCorner(nFrac,nFrac);
    matrixData.getBlock({2,0})=matrixData.getBlock({1,0}).bottomLeftCorner(nFrac,nVel);
    matrixData.getBlock({1,0})=matrixData.getBlock({1,0}).topLeftCorner(nCell,nVel);
    matrixData.addTranspose({0,2},{2,0});
    matrixData.getBlock({1,1}).resize(nCell,nCell);
    matrixData.getBlock({1,2}).resize(nCell,nFrac);
    matrixData.getBlock({2,1}).resize(nFrac,nCell);
    matrixData.changeOffsets({nVel,Eigen::Index(nCell),Eigen::Index(nFrac)},{nVel,Eigen::Index(nCell),Eigen::Index(nFrac)});
/*
    for (auto i=0;i<3;++i)
      for (auto j=0;j<3;++j)
        {
          std::clog<<"Block ("<<i<<","<<j<<")\n";
          std::clog<<"Rows declared"<<matrixData.rows({i,j})<<"Effective "<<matrixData.getBlock({i,j}).rows()<<std::endl;;
          std::clog<<"Cols declared"<<matrixData.cols({i,j})<<"Effective "<<matrixData.getBlock({i,j}).cols()<<std::endl;

        }
*/
  }

}// end namespace





#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTMAT_HPP_ */
