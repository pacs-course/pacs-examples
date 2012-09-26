#ifndef EIGENREADMM_HPP__
#define EIGENREADMM_HPP__
#include <vector>
#include <string>
#include <Eigen/SparseCore>
#include "mmio.hpp"
namespace EigenReaders{
  
  //! Reads a sparse Eigen Matrix from a matrix market file.
  /*!
    The matrix must be a Eigen Sparse Matrix of doubles.
  */
  template<int O, typename IT>
  void readFromMMFile(Eigen::SparseMatrix<double,O,IT> & mat,
		 const std::string filename)
  {
    typedef Eigen::Triplet<double,IT> triplet;
    
    int N, M,nz;
    std::vector<double> val;
    std::vector<int> I;
    std::vector<int> J;
    
    MMIO::mm_read_unsymmetric_sparse(filename,M, N,nz,val,I,J);
    
    // Not very efficient;
    // I need to use this strange way of building sparse matrices
    // used by Eigen
    std::vector<triplet> eVal(nz);
    for (int i=0; i<nz; ++i) eVal[i]=triplet(I[i],J[i],val[i]);
    // Clear up memory!
    val.clear();
    I.clear();
    J.clear();
    val.shrink_to_fit(); /// C++11!
    I.shrink_to_fit();
    J.shrink_to_fit();
    mat.resize(M,N);
    mat.setFromTriplets(eVal.begin(), eVal.end());
    return;
  }
}
#endif
