/*
 * dumpMatrices.cpp
 *
 *  Created on: Dec 4, 2018
 *      Author: forma
 */

#include "dumpMatrices.hpp"

namespace FVCode3D
{
bool
dumpSaddlePointMatrix(const SaddlePointMat &M)
{
  bool status{true};
  status = dumpSparseMatrix(M.getM(), "M.mat", true);
  if(!status)
    return false;
  status = dumpSparseMatrix(M.getB(), "B.mat", false);
  if(!status)
    return false;
  status = dumpSparseMatrix(M.getT(), "T.mat", true);
  return status;
}

bool
dumpApproximateSchurMatrix(const SaddlePointMat &SP, bool lumping)
{
  FVCode3D::DiagMat MRed;
  if(lumping)
    {
      auto & M = SP.getM();
      Vector ML(M.rows());
      ML.setZero();
      for(int j = 0; j < M.outerSize(); ++j)
        {
          for(SpMat::InnerIterator it(M, j); it; ++it)
            {
              ML[it.row()] += it.value();
            }
        }
      MRed = ML.asDiagonal();
    }
  else
    {
      MRed = SP.getM().diagonal().asDiagonal();
    }
  FVCode3D::SpMat AShur = -SP.getB() * MRed.inverse() * SP.getB().transpose();
  AShur += SP.getT(); // Note is -T in the paper!
  bool                                status{true};
  std::vector<Eigen::Triplet<double>> trip;
  trip.reserve(MRed.rows());
  for(int i = 0; i < MRed.rows(); ++i)
    trip.emplace_back(i, i, MRed.diagonal()[i]);
  FVCode3D::SpMat MRedS(MRed.rows(), MRed.rows());
  MRedS.setFromTriplets(trip.begin(), trip.end());
  status = FVCode3D::dumpSparseMatrix(MRedS, "MApprox.mat", true);
  if(!status)
    return false;
  status = FVCode3D::dumpSparseMatrix(AShur, "ShurApprox.mat", true);
  return status;
}
} // namespace FVCode3D
