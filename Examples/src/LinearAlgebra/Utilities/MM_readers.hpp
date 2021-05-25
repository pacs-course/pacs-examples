// This file is an extension Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2017 Luca Formaggia <luca.formaggia@polimi.it>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// It is a simplification (and extension) of MatrixMarketIterator
// developed by Desire NUENTSA WAKAM <desire.nuentsa_wakam@inria.fr>
// 
//
#ifndef HH_EIGEN__MM_READERS_HPP
#define HH_EIGEN__MM_READERS_HPP
#include <exception>
#include "Eigen/Core"

#if (EIGEN_MAJOR_VERSION < 3) || (EIGEN_WORLD_VERSION < 3)
#error "I need EIGEN at least Version 3.3.X"
#endif

#include "unsupported/Eigen/SparseExtra"
// This is for version 3.3.3 or less only
#if (EIGEN_MINOR_VERSION <= 3)
namespace Eigen
{
  /*!
    Reads a sparse matrix from a file in matrix market format.
    In this version the output matrix has a full graph even if it is 
    declared symmetric in the file.
    
    \tparam SparseMatrixType A Eigen sparse matrix type
    \param filename The name of the file.
  */
  template<typename SparseMatrixType>
  SparseMatrixType read_MM_Matrix(const std::string& filename);

  /*!
    Reads an Eigen vector. It returns an Eigen vector 
    It does not work with std::vector.
    \tparam VectorType. A Eigen vector type. 
    \param filename. The name of the file storing the vector
  */
  template<typename VectorType>
  VectorType read_MM_Vector(const std::string& filename);
  
  /* To write Matrices or Vectors in MatrixMarket format use
     from unsupported/Eigen/SparseExtra:
     
     template<typename SparseMatrixType>
     bool saveMarket(const SparseMatrixType& mat, const std::string& filename, int sym = 0)
     
     template<typename VectorType>
     bool saveMarketVector (const VectorType& vec, const std::string& filename)
  */

  
  // ***   IMPLEMENTATIONS  ****
  template<typename SparseMatrixType>
  SparseMatrixType read_MM_Matrix(const std::string& filename)
  {
    typedef typename SparseMatrixType::Scalar Scalar;
    typedef typename SparseMatrixType::Index Index;
    SparseMatrixType mat;
    bool symmetric=false;
    std::regex rsym(" symmetric",std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::ifstream input(filename.c_str(),std::ios::in);
    if(!input)
      throw std::runtime_error(std::string("Cannot open file ")+filename);
    
    const int maxBuffersize = 2048;
    char buffer[maxBuffersize];
    
    bool readsizes = false;
    
    typedef Triplet<Scalar,Index> T;
    std::vector<T> elements;
    
    Index M(-1), N(-1), NNZ(-1);
    Index count = 0;
    while(input.getline(buffer, maxBuffersize))
      {
        // Added by Luca Formaggia: test for symmetric matrices
        // If the file indicates that the matrix is symmetric
        // only the upper diagonal (or lower duiagonal) part is actually stored
        // so we need to double the entries.
        if(buffer[0]=='%')
          {
            symmetric=symmetric || std::regex_search(std::string(buffer),rsym);
            continue;
          }
        
    std::stringstream line(buffer);
    
    if(!readsizes)
      {
        line >> M >> N >> NNZ;
        int nonZeros;
        if(symmetric)
          {
            if (M != N)
              {
                throw std::runtime_error(std::string("Error while reding matrix. A symmetric matrix must be square"));
              }
            nonZeros=2*NNZ-M;
          }
        else
          {
            nonZeros=NNZ;
          }
        
        if(M > 0 && N > 0 && NNZ > 0) 
          {
            readsizes = true;
            //std::cout << "sizes: " << M << "," << N << "," << NNZ << "\n";
            mat.resize(M,N);
            mat.reserve(nonZeros);
          }
      }
    else
      { 
        Index i(-1), j(-1);
        Scalar value; 
        if( internal::GetMarketLine(line.str(), M, N, i, j, value) )
        {
          ++ count;
          elements.push_back(T(i,j,value));
          // In the case of symmetric matriced
          // it'd be better to test if (j,i) is already inserted.
          // In this case, it is an error since the file should
          // contain only the upper or lower diagonal
          if(symmetric && (i!=j))elements.push_back(T(j,i,value));
        }
      else
        {
          std::ostringstream err;
          err<<"Invalid read: " << i << "," << j<<" in file "<<filename;
          throw std::runtime_error(err.str());
        }
    }
      }
    mat.setFromTriplets(elements.begin(), elements.end());
    if(count!=NNZ)
    {
      std::ostringstream err;
      err<<"Wrong number of non zero entries: "<< NNZ << " in file "<<filename;
      throw std::runtime_error(err.str());
    }
    input.close();
    return mat;
  }

  template<typename VectorType>
  VectorType read_MM_Vector(const std::string& filename)
  {
    typedef typename VectorType::Scalar Scalar;
    VectorType vec;
    std::ifstream in(filename.c_str(), std::ios::in);
    if(!in)
      throw std::runtime_error(std::string("Cannot open file ")+filename);;
    
    std::string line; 
    int n(0), col(0); 
    do 
      { // Skip comments
        std::getline(in, line); eigen_assert(in.good());
      } while (line[0] == '%');
    std::istringstream newline(line);
    newline  >> n >> col; 
    eigen_assert(n>0 && col>0);
    vec.resize(n);
    int i = 0; 
    Scalar value; 
    while ( std::getline(in, line) && (i < n) ){
      internal::GetVectorElt(line, value); 
      vec(i++) = value; 
    }
    in.close();
    if (i!=n){
      throw std::runtime_error(std::string("Unable to read all elements from file ") + filename);
    }
    return vec;
  }
}
// This is for version > 3.3.3
#else
#include <type_traits>
namespace
{
  /*!
    My modification for symmetric matrices
   */
  template<typename SparseMatrixType>
  bool symLoadMarket(SparseMatrixType& mat, const std::string& filename)
  {
    typedef typename SparseMatrixType::Scalar Scalar;
    typedef typename SparseMatrixType::Index StorageIndex;
    std::ifstream input(filename.c_str(),std::ios::in);
    if(!input)
      throw std::runtime_error(std::string("Error in opening file ")+filename);
    //    return false;
    
    char rdbuffer[4096];
    input.rdbuf()->pubsetbuf(rdbuffer, 4096);
    
    const int maxBuffersize = 2048;
    char buffer[maxBuffersize];
    
    bool readsizes = false;
    
    typedef Eigen::Triplet<Scalar,StorageIndex> T;
    std::vector<T> elements;
    
    Eigen::Index M(-1), N(-1), NNZ(-1);
    Eigen::Index count = 0;
    while(input.getline(buffer, maxBuffersize))
      {
        if(buffer[0]=='%')
          continue;
        
        if(!readsizes)
          {
            std::stringstream line(buffer);
            line >> M >> N >> NNZ;
            if(M > 0 && N > 0 && NNZ > 0) 
              {
                if( N != M) throw std::runtime_error("Symmetric matricies should be square");
                readsizes = true;
                mat.resize(M,N);
                // I store the whole matrix for simplicity
                // Eigen has tools to view a symmetric matrix that stores
                // only the upper/lower triangular part, but using this feature
                // make handling the different cases complicated
                // Here NNZ is the number of lines to be read in the file
                mat.reserve(2*NNZ-N);
              }
          }
        else
          { 
            StorageIndex i{0}, j(0);
            Scalar value; 
            std::stringstream line(buffer);
            line>>i>>j>>value;
            if(line.good())
            {
              ++count;
              elements.push_back(T(i,j,value));
              if(i != j) elements.push_back(T(j,i,value));
            }
            else
              {
                std::cerr << "Invalid read: " << i << "," << j << "\n";
                throw std::runtime_error("Matrix file incorrectly formatted");
              }
          }
      }
    
    mat.setFromTriplets(elements.begin(), elements.end());
    if(count!=NNZ)
      {
        std::cerr << count << "!=" << NNZ << "\n";
        throw std::runtime_error("Matrix file incorrectly formatted");
      }
    
    input.close();
    
    return true;
  }
}

namespace Eigen
{
  /*!
    Reads a sparse matrix from a file in matrix market format.
    In this version the output matrix has a full graph even if it is 
    declared symmetric in the file.
    
    \param SparseMatrixType A Eigen sparse matrix type
    \param filename The name of the file.
  */
  
  template<typename SparseMatrixType>
  SparseMatrixType read_MM_Matrix(const std::string& filename);
  /*!
    Reads an Eigen vector. It returns an Eigen vector 
    It does not work with std::vector.
    \param Vector type. A Eigen vector type. 
    \param filename. The name of the file storing the vector
  */
  template<typename VectorType>
  VectorType read_MM_Vector(const std::string& filename);
  
  /* To write Matrices or Vectors in MatrixMarket format use
     from unsupported/Eigen/SparseExtra:
     
     template<typename SparseMatrixType>
     bool saveMarket(const SparseMatrixType& mat, const std::string& filename, int sym = 0)
     
     template<typename VectorType>
     bool saveMarketVector (const VectorType& vec, const std::string& filename)
  */

  
  // ***   IMPLEMENTATIONS  ****
  template<typename SparseMatrixType>
  SparseMatrixType read_MM_Matrix(const std::string& filename)
  {
    //typedef typename SparseMatrixType::Scalar Scalar;
    // more C++11 syntax
    using Scalar=typename SparseMatrixType::Scalar;
    bool iscomplex(false);
    bool isvector(false);
    int sym;
    SparseMatrixType mat;
    bool good=getMarketHeader(filename, sym, iscomplex, isvector);
    bool symmetric=(sym==Eigen::Symmetric || sym==Eigen::SelfAdjoint);
#ifdef DEBUG
    if (symmetric) std::cout<<"Symmetric matrix"<<std::endl;
    else std::cout<<"Unymmetric matrix"<<std::endl;
    if (iscomplex) std::cout<<"Complex matrix"<<std::endl;
    else std::cout<<"Real matrix"<<std::endl;
#endif
    if(isvector)
      {
        throw
          std::runtime_error(std::string("read_MM_Matrix cannot read a vector"));
      }
    if (iscomplex)
      {
        if (!std::is_same<std::complex<double>,Scalar>::value)
          {
            throw
              std::runtime_error(std::string("read_MM_Matrix: wrong matrix type: read_MM_Matrix is only for matrices of reals"));
          }
      }
      /*
        else
        Not neded I will convert double to complex
        {
        if (!std::is_same<double,Scalar>::value)
        {
        throw
              std::runtime_error(std::string("read_MM_Matrix: wrong matrix type: it should store double"));          
        }
      }      
      */
      
    if(! symmetric)
      {
        // use standard reader
        good=Eigen::loadMarket(mat, filename);
      }
    else
      {
        good=symLoadMarket(mat, filename);
      }
    if(!good)  throw
        std::runtime_error(std::string("read_MM_Matrix: CANNOT READ MATRIX"));  
    return mat;
  }
  
  template<typename VectorType>
  VectorType read_MM_Vector(const std::string& filename)
  {
    VectorType vec;
    auto good = Eigen::loadMarketVector(vec,filename);
    if(!good)
      throw std::runtime_error(std::string("Cannot read vector ")+filename);;
    return vec;
  }
}
#endif
#endif
			  
