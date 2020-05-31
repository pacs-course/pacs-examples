/*!
 * @file preconditioner.hpp
 * @brief These classes implement simple classes that perform the "inversion" of the preconditioner (definition version)
*/

#include "preconditioner.hpp"
#include "BasicType.hpp"
#include "chrono.hpp"
#include <Eigen/LU>
#include <unsupported/Eigen/SparseExtra>
#include <chrono>
#include <cmath>
#include <iostream>
#include "cg.hpp"
namespace FVCode3D
{
  // LF si potrebbe usare decltype(auto)?
  DiagMat ComputeApproximateInverseInnerProd(const SaddlePointMat & SP,
					     bool lumping)
  {
    if (lumping)
      {
        std::cout<<"Using lumped M matrix"<<std::endl;
	auto & M = SP.getM();
	Vector ML(M.rows());
	ML.setZero();
	for(int j = 0; j<M.outerSize(); ++j)
	  {
	    for(SpMat::InnerIterator it(M,j); it; ++it)
	      {
		ML[it.row()] += it.value();
	      }
	  }
	return ML.asDiagonal().inverse();
      }
    else
    {
      std::cout<<"Using diagonal of M matrix"<<std::endl;
      return SP.getM().diagonal().asDiagonal().inverse();
    }        
  }

  SpMat ComputeApproximateSchur(const SaddlePointMat & SP,
				const DiagMat & D)
  {
    // T - B D^{-1} B^T
    SpMat res=- SP.getB() * D * SP.getB().transpose();
    res+=SP.getT();
    return res;
  }

  SpMat SaddlePointMat::fullMatrix() const
  {
    long int const nrows=M.rows()+B.rows();
    long int const Msize=M.rows();
    SpMat result(nrows,nrows);
    long int const nnz=static_cast<long int >(this->nonZeros());
    result.reserve(nnz);
    std::vector< Eigen::Triplet<double,long int> > tripletList;
    tripletList.reserve(nnz);
    double s= isSymUndef? 1.0: -1.0;
    for (int k=0; k<M.outerSize(); ++k)
      for (SpMat::InnerIterator it(M,k); it; ++it)
	{
	  tripletList.emplace_back(Eigen::Triplet<double,long int>{it.row(),it.col(),it.value()});
	}
    for (int k=0; k<B.outerSize(); ++k)
      for (SpMat::InnerIterator it(B,k); it; ++it)
	{
	  tripletList.emplace_back(Eigen::Triplet<double,long int>{it.row()+Msize,it.col(),s*it.value()} );
	  tripletList.emplace_back(Eigen::Triplet<double,long int>{it.col(),it.row()+Msize,it.value()} );
	}

    for (int k=0; k<T.outerSize(); ++k)
      for (SpMat::InnerIterator it(T,k); it; ++it)
	{
	  tripletList.emplace_back(Eigen::Triplet<double,long int>{it.row()+Msize,it.col()+Msize,s*it.value()} );
	}
    result.setFromTriplets(tripletList.begin(), tripletList.end());
    return result;
  }

  void SaddlePointMat::clear()
  {
  M.resize(0,0);
  B.resize(0,0);
  T.resize(0,0);
  M.data().squeeze();
  B.data().squeeze();
  T.data().squeeze();
  }

  Vector diagonal_preconditioner::solve(const Vector & r) const
  {
    auto & M = *Mptr;
    auto & T = *Tptr;
    Vector z = Vector::Zero(M.rows()+T.rows());      // The preconditioned residual
    for(int i = 0; i<M.rows(); i++)
      {
	if(M.coeff(i,i)!=0)
	  z[i] = r[i]/std::abs(M.coeff(i,i));
	else
	  // This should never happen, preconditioner must be non singular
	  z[i] = r[i];
      }
    for(int i = 0; i<T.rows(); i++)
      {
	if(T.coeff(i,i)!=0)
	  z[M.rows()+i] = r[M.rows()+i]/std::abs(T.coeff(i,i));
	else
	  // This may happen since T may be zero.
	  z[M.rows()+i] = r[M.rows()+i];
      }
    return z;
  }

  Vector BlockDiagonal_preconditioner::solve(const Vector & r) const
  {
    auto & B = *Bptr;
    Vector z(Md_inv.rows()+B.rows());
    // First step: solve Inexact Schur Complement linear system
    z.head(Md_inv.rows()) = Md_inv*(r.head(Md_inv.rows()));
    // Apply -Shur complement to pressure residual
    z.tail(B.rows()) = chol.solve(r.tail(B.rows()));
    return z;
  }

  Vector BlockTriangular_preconditioner::solve(const Vector & r) const
  {
    auto & B = *Bptr;
    // First step: solve Inexact Schur Complement linear system
    // Apply -Shur complement to pressure residual
    Vector y2 = chol.solve(r.tail(B.rows()));
    // Second step: solve the diagonal linear system
    Vector z(Md_inv.rows()+B.rows());
    z.head(Md_inv.rows()) = Md_inv*(r.head(Md_inv.rows())+B.transpose()*y2);
    z.tail(B.rows()) = -y2;
    return z;
  }

  Vector ILU_preconditioner::solve(const Vector & r) const
  {
    auto & B = *Bptr;
    // First step: solve the 1st diagonal linear system
    Vector y1 = Md_inv*r.head(Md_inv.rows());
    // Second step: solve the ISC linear system
    Vector y2 = chol.solve(B*y1-r.tail(B.rows()));
    // Third step: solve the 2nd diagonal linear system
    Vector z(Md_inv.rows()+B.rows());
    z.head(Md_inv.rows()) = y1-Md_inv*B.transpose()*y2;
    z.tail(B.rows()) = y2;
    return z;
  }
  /* with c++17 is not necessary anymore
   * and anyway it is required only if the constant is odr used, which is not the case
   * here. Since since c++17 out-of-class definition of constexprs is deprecated and since
   * we do not odr use them here, I just comment the definitions
   *
UInt constexpr HSS_preconditioner::MaxIt_default;
Real constexpr HSS_preconditioner::tol_default;
Real constexpr HSS_preconditioner::alpha_default;
   */

  void HSS_preconditioner::set(const SaddlePointMat & SP)
  {
    auto & M = SP.getM();
    auto & B = SP.getB();
    auto & T = SP.getT();
    Bptr = & SP.getB();
    Halpha = M;
    for(int i=0; i<M.rows(); i++)
      Halpha.coeffRef(i,i) += alpha;

    UInt c  = 0;   //To count number of fracture facets
    UInt cc = 0;
    for (int k=0; k<T.outerSize(); ++k)
      {
	for (SpMat::InnerIterator it(T,k); it; ++it)
	  {
	    if(it.value() != 0)
	      {
		cc++;
		if(cc == 1)
		  c++;
	      }
	  }
	cc = 0;
      }
    Nfrac = c;
    Ncell = T.rows()-c;
    SpMat Talpha = -T.bottomRightCorner(Nfrac,Nfrac);
    for(int i=0; i<Talpha.rows(); i++)
      Talpha.coeffRef(i,i) += alpha;

    SpMat BBtalpha = B*B.transpose();
    for(int i=0; i<BBtalpha.rows(); i++)
      BBtalpha.coeffRef(i,i) += alpha*alpha;

    //cg.setMaxIterations(MaxIt);
    //cg.setTolerance(tol);
    //cg.compute(Halpha);
    cholT.compute(Talpha);
    cholBBt.compute(BBtalpha);
  }

  Vector HSS_preconditioner::solve(const Vector & r) const
  {
    auto & B = *Bptr;
    Vector rscaled=2*alpha*r;
    // First step: solve the H linear system
    //Vector omega1 = cg.solve(rscaled.head(Halpha.rows()));
    Eigen::DiagonalPreconditioner<double> D(Halpha);
    Vector omega1(Halpha.rows());
    omega1.fill(0.0);
    auto tolrcg=tol;
    int maxitcg=MaxIt;
    Vector b = rscaled.head(Halpha.rows());
    LinearAlgebra::CG(Halpha,omega1,b, D, maxitcg, tolrcg);
    // Second step: solve the T linear system
    Vector omega2(Ncell+Nfrac);
    omega2.tail(Nfrac) = cholT.solve(rscaled.tail(Nfrac));
    for(UInt i = 0; i<Ncell; i++)
     {
  	omega2[i] = 2.*r[Halpha.rows()+i];
     }
    // Third step: solve the BBt linear system
    Vector z(Halpha.rows()+Ncell+Nfrac);
    z.tail(Ncell+Nfrac) = cholBBt.solve(B*omega1+alpha*omega2);
    z.head(Halpha.rows()) = (omega1-B.transpose()*z.tail(Ncell+Nfrac))/alpha;
    return z;
  }

}
