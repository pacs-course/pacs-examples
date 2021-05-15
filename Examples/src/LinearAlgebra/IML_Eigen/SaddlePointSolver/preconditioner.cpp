/*!
 * @file preconditioner.hpp
 * @brief These classes implement simple classes that perform the "inversion" of
 * the preconditioner (definition version)
 */

#include "BasicType.hpp"
#include "cg.hpp"
#include "chrono.hpp"
#include "preconditioner.hpp"
#include <Eigen/LU>
#include <unsupported/Eigen/SparseExtra>
#include <chrono>
#include <cmath>
#include <iostream>
namespace FVCode3D
{
DiagMat
ComputeApproximateInverseInnerProd (const SaddlePointMat &SP, bool lumping)
{
  if (lumping)
    {
      std::cout << "Using lumped M matrix" << std::endl;
      auto & M = SP.getM ();
      Vector ML (M.rows ());
      ML.setZero ();
      for (int j = 0; j < M.outerSize (); ++j)
        {
          for (SpMat::InnerIterator it (M, j); it; ++it)
            {
              ML[it.row ()] += std::abs(it.value ());
            }
        }
      return ML.asDiagonal ().inverse ();
    }
  else
    {
      std::cout << "Using diagonal of M matrix" << std::endl;
      return SP.getM ().diagonal ().asDiagonal ().inverse ();
    }
}

SpMat
ComputeApproximateSchur (const SaddlePointMat &SP, const DiagMat &D)
{
  // T - B D^{-1} B^T
  SpMat res = -SP.getB () * D * SP.getB ().transpose ();
  res += SP.getT ();
  return res;
}

SpMat
  ComputeApproximateSchurDSP (const SaddlePointMat &Mat, const DiagMat &D)
  {
  apsc::SparseBlockMatrix<double,3,3> const & matrix=Mat.sparseBlockMatrix();
  SpMat res =-matrix.getBlock({0,2})*D*matrix.getBlock({0,2}).transpose();
  res+=matrix.getBlock({2,2});
  return res;
  }

SpMat
  BamBt (const SaddlePointMat &Mat, const DiagMat &D)
  {
  apsc::SparseBlockMatrix<double,3,3> const & matrix=Mat.sparseBlockMatrix();
  return -matrix.getBlock({1,0})*D*matrix.getBlock({1,0}).transpose();
  }

SpMat
  BamCt (const SaddlePointMat &Mat, const DiagMat &D)
  {
  apsc::SparseBlockMatrix<double,3,3> const & matrix=Mat.sparseBlockMatrix();
  return matrix.getBlock({1,0})*D*matrix.getBlock({2,0}).transpose();
  }


Vector
diagonal_preconditioner::solve (const Vector &r) const
{
  auto & M = *Mptr;
  auto & T = *Tptr;
  Vector z =
    Vector::Zero (M.rows () + T.rows ()); // The preconditioned residual
  for (int i = 0; i < M.rows (); i++)
    {
      if (M.coeff (i, i) != 0)
        z[i] = r[i] / std::abs (M.coeff (i, i));
      else
        // This should never happen, preconditioner must be non singular
        z[i] = r[i];
    }
  for (int i = 0; i < T.rows (); i++)
    {
      if (T.coeff (i, i) != 0)
        z[M.rows () + i] = r[M.rows () + i] / std::abs (T.coeff (i, i));
      else
        // This may happen since T may be zero.
        z[M.rows () + i] = r[M.rows () + i];
    }
  return z;
}

Vector
BlockDiagonal_preconditioner::solve (const Vector &r) const
{
  auto & B = *Bptr;
  Vector z (Md_inv.rows () + B.rows ());
  // First step: solve Inexact Schur Complement linear system
  z.head (Md_inv.rows ()) = Md_inv * (r.head (Md_inv.rows ()));
  // Apply -Shur complement to pressure residual
  z.tail (B.rows ()) = chol.solve (r.tail (B.rows ()));
  return z;
}

Vector
BlockTriangular_preconditioner::solve (const Vector &r) const
{
  auto &B = *Bptr;
  // First step: solve Inexact Schur Complement linear system
  // Apply -Shur complement to pressure residual
  Vector y2 = chol.solve (r.tail (B.rows ()));
  // Second step: solve the diagonal linear system
  Vector z (Md_inv.rows () + B.rows ());
  z.head (Md_inv.rows ()) =
    Md_inv * (r.head (Md_inv.rows ()) + B.transpose () * y2);
  z.tail (B.rows ()) = -y2;
  return z;
}

Vector
ILU_preconditioner::solve (const Vector &r) const
{
  auto &B = *Bptr;
  // First step: solve the 1st diagonal linear system
  Vector y1 = Md_inv * r.head (Md_inv.rows ());
  // Second step: solve the ISC linear system
  Vector y2 = chol.solve (B * y1 - r.tail (B.rows ()));
  // Third step: solve the 2nd diagonal linear system
  Vector z (Md_inv.rows () + B.rows ());
  z.head (Md_inv.rows ()) = y1 - Md_inv * B.transpose () * y2;
  z.tail (B.rows ()) = y2;
  return z;
}
/* with c++17 is not necessary anymore
 * and anyway it is required only if the constant is odr used, which is not the
case
 * here. Since since c++17 out-of-class definition of constexprs is deprecated
and since
 * we do not odr use them here, I just comment the definitions
 *
UInt constexpr HSS_preconditioner::MaxIt_default;
Real constexpr HSS_preconditioner::tol_default;
Real constexpr HSS_preconditioner::alpha_default;
 */

void
HSS_preconditioner::set (const SaddlePointMat &SP)
{
  auto &M = SP.getM ();
  auto &B = SP.getB ();
  auto &T = SP.getT ();
  Bptr = &SP.getB ();
  //! scaling M
  this->scaledM.resize (M.rows ());
  this->scaledM.setZero ();
  double scaling2 = 1.0 / static_cast<double> (M.rows ());
  Halpha = M;
  for (int k = 0; k < Halpha.outerSize (); ++k)
    {
      for (SpMat::InnerIterator it (Halpha, k); it; ++it)
        {
          auto i = it.row ();
          // auto j = it.col();
          this->scaledM (i) += it.value () * it.value ();
        }
    }
  scaledM.array () = scaling2 / scaledM.array ().sqrt ();
  Halpha = scaledM.asDiagonal () * Halpha;

  for (int i = 0; i < M.rows (); i++)
    Halpha.coeffRef (i, i) += alpha;

  UInt c = 0; // To count number of fracture facets
  UInt cc = 0;
  for (int k = 0; k < T.outerSize (); ++k)
    {
      for (SpMat::InnerIterator it (T, k); it; ++it)
        {
          if (it.value () != 0)
            {
              cc++;
              if (cc == 1)
                c++;
            }
        }
      cc = 0;
    }
  Nfrac = c;
  Ncell = T.rows () - c;
  SpMat Talpha = -T.bottomRightCorner (Nfrac, Nfrac);
  for (int i = 0; i < Talpha.rows (); i++)
    Talpha.coeffRef (i, i) += alpha;

  SpMat BBtalpha = B * B.transpose ();
  for (int i = 0; i < BBtalpha.rows (); i++)
    BBtalpha.coeffRef (i, i) += alpha * alpha;

  // cg.setMaxIterations(MaxIt);
  // cg.setTolerance(tol);
  // cg.compute(Halpha);
  cholT.compute (Talpha);
  cholBBt.compute (BBtalpha);
}

Vector
HSS_preconditioner::solve (const Vector &r) const
{
  auto & B = *Bptr;
  Vector rscaled = 2.0 * alpha * r;
  // First step: solve the H linear system
  // Vector omega1 = cg.solve(rscaled.head(Halpha.rows()));
  Eigen::DiagonalPreconditioner<double> D (Halpha);
  Vector                                omega1 (Halpha.rows ());
  omega1.fill (0.0);
  auto   tolrcg = tol;
  int    maxitcg = MaxIt;
  Vector b = this->scaledM.asDiagonal () * rscaled.head (Halpha.rows ());
  LinearAlgebra::CG (Halpha, omega1, b, D, maxitcg, tolrcg);
  // Second step: solve the T linear system
  Vector omega2 (Ncell + Nfrac);
  omega2.tail (Nfrac) = cholT.solve (rscaled.tail (Nfrac));
  for (UInt i = 0; i < Ncell; i++)
    {
      omega2[i] = 2. * r[Halpha.rows () + i];
    }
  // Third step: solve the BBt linear system
  Vector z (Halpha.rows () + Ncell + Nfrac);
  z.tail (Ncell + Nfrac) = cholBBt.solve (B * omega1 + alpha * omega2);
  z.head (Halpha.rows ()) =
    (omega1 - B.transpose () * z.tail (Ncell + Nfrac)) / alpha;
  return z;
}

Vector
 DoubleSaddlePoint_preconditioner::solve (const Vector &r) const
 {
  auto const & B=*Bptr;
  auto const & C=*Cptr;
  Vector z(nVel+nCell+nFrac);
  z.segment(nVel+nCell,nFrac)=Schur_chol.solve(r.segment(nVel+nCell,nFrac));
  z.segment(nVel,nCell)      =BAB_chol.solve(r.segment(nVel,nCell)+BAmC*z.segment(nVel+nCell,nFrac));
  z.segment(0,nVel)          =Md_inv*(r.segment(0,nVel)-B.transpose()*z.segment(nVel,nCell)-C.transpose()*z.segment(nVel+nCell,nFrac));
  return z;
 }

  Vector
  DoubleSaddlePointSym_preconditioner::solve (const Vector &r) const
  {
    auto const & B=*Bptr;
    auto const & C=*Cptr;
    Vector z(nVel+nCell+nFrac);
    z.segment(nVel+nCell,nFrac)=Schur_chol.solve(-r.segment(nVel+nCell,nFrac));
    z.segment(nVel,nCell)      =BAB_chol.solve(-r.segment(nVel,nCell));
    z.segment(0,nVel)          =Md_inv*(r.segment(0,nVel));
    return z;
  }

}
