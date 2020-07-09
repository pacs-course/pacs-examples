/*
 * SaddlePointMat.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: forma
 */
#include "SaddlePointMat.hpp"
namespace FVCode3D
{
SpMat
SaddlePointMat::fullMatrix () const
{
  long int const nrows = M.rows () + B.rows ();
  long int const Msize = M.rows ();
  SpMat          result (nrows, nrows);
  long int const nnz = static_cast<long int> (this->nonZeros ());
  result.reserve (nnz);
  std::vector<Eigen::Triplet<double, long int> > tripletList;
  tripletList.reserve (nnz);
  double s = isSymUndef ? 1.0 : -1.0;
  for (int k = 0; k < M.outerSize (); ++k)
    for (SpMat::InnerIterator it (M, k); it; ++it)
      {
        tripletList.emplace_back (
          Eigen::Triplet<double, long int>{it.row (), it.col (), it.value ()});
      }
  for (int k = 0; k < B.outerSize (); ++k)
    for (SpMat::InnerIterator it (B, k); it; ++it)
      {
        tripletList.emplace_back (Eigen::Triplet<double, long int>{
          it.row () + Msize, it.col (), s * it.value ()});
        tripletList.emplace_back (Eigen::Triplet<double, long int>{
          it.col (), it.row () + Msize, it.value ()});
      }

  for (int k = 0; k < T.outerSize (); ++k)
    for (SpMat::InnerIterator it (T, k); it; ++it)
      {
        tripletList.emplace_back (Eigen::Triplet<double, long int>{
          it.row () + Msize, it.col () + Msize, s * it.value ()});
      }
  result.setFromTriplets (tripletList.begin (), tripletList.end ());
  return result;
}

void
SaddlePointMat::clear ()
{
  M.resize (0, 0);
  B.resize (0, 0);
  T.resize (0, 0);
  M.data ().squeeze ();
  B.data ().squeeze ();
  T.data ().squeeze ();
}

}// end namespace


