#ifndef HH_COO_EXTRACTOR_HPP
#define HH_COO_EXTRACTOR_HPP
#include "Eigen/SparseCore"

#include <array>
#include <iostream>    // for i/o
#include <tuple>       // for std::tie
#include <type_traits> // for some static checks on types
#include <utility>     // for pairs (useles since we have tuple
#include <vector>

namespace LinearAlgebra
{
  //! An alias to vector to store a matrix indices
  template <class Index = unsigned int>
  using CooIndices = std::vector<std::array<Index, 2>>;

  //! An alias for the vector storing array elements associated to
  //! an object of CooIndices type
  template <class Value = double>
  using CooMatrixElements = std::vector<Value>;

  //! An alias to a pair of indices and values in COO format
  template <class Index = unsigned int, class Value = double>
  using CooData = std::pair<CooIndices<Index>, CooMatrixElements<Value>>;
  //! Write in a format readable from Matlab and Octave
  /*!
    \param out output stream (typically a file)
    \param coo The structure containg the data in COO format
    \param cols Number of columns
    \param rows Number of rows
    \return The stream for concatenation purposes
   */
  template <class Index, class Value>
  std::ostream &
  outputForOctave(std::ostream &               out,
                  CooData<Index, Value> const &coo,
                  int                          rows,
                  int                          cols);

  /*! Write in a format readable from Matlab and Octave

     Version for Eigen Matrices

    \param out output stream (typically a file)
    \param m An Eigen Matrix
    \return The stream for concatenation purposes
   */
  template <class EMat>
  std::ostream &
  outputForOctave(std::ostream &out, EMat const &m);

  /*!
    Extract matrix in coordinate format
    \param mat A sparse Eigen matrix
    \return The structure with the info of the coo format
   */
  template <class SpMat>
  auto
  extractCooData(SpMat const &mat)
    -> CooData<typename SpMat::Index, typename SpMat::Scalar>;

  /*!
    Prints the matrix
    \param out An output stream
    \param coo the structure containing the matrix as COO data
    \return The output stream
   */
  template <class Index, class Value>
  std::ostream &
  printCooData(std::ostream &out, CooData<Index, Value> const &coo);

  /*!
    Prints the matrix by calling \ref{printCooData}
    \param out An output stream
    \param coo the structure containing the matrix as COO data
    \return The output stream
   */
  template <class Index, class Value>
  std::ostream &
  operator<<(std::ostream &out, CooData<Index, Value> const &coo);

  /*!
    Creates an Eigen matrix from a CooData structure
    /param SparseEigenMatrix The type of the sparse Eigen Matrix
    /param Index the type used to store indices
    /param cooMat The CooData structure
    /return An Eigen Matrix
  */
  template <class SparseEigenMatrix, class Index, class Value>
  SparseEigenMatrix
  make_EigenMatrix(CooData<Index, Value> const &cooMat);


  //   IMPLEMENTATIONS START HERE
  template <class SpMat>
  auto
  extractCooData(SpMat const &mat)
    -> CooData<typename SpMat::Index, typename SpMat::Scalar>
  {
    using Index                   = typename SpMat::Index;
    using Scalar                  = typename SpMat::Scalar;
    auto                      nnz = mat.nonZeros();
    CooIndices<Index>         indices;
    CooMatrixElements<Scalar> values;
    indices.reserve(nnz);
    values.reserve(nnz);
    //! I need to get ordering
    for (Index k = 0; k < mat.outerSize(); ++k)
      for (typename SpMat::InnerIterator it(mat, k); it; ++it)
        {
          indices.emplace_back(std::array<Index, 2>{{it.row(), it.col()}});
          values.emplace_back(it.value());
        }
    // it can be optimized by creating the CooData object from the
    // start!
    return CooData<Index, Scalar>{indices, values};
  };

  template <class Index, class Value>
  std::ostream &
  printCooData(std::ostream &out, CooData<Index, Value> const &coo)
  {
    auto const &values  = coo.second;
    auto const &indices = coo.first;
    Index       c       = 0;
    for (auto i : indices)
      {
        out << "(" << i[0] << ", " << i[1] << ")"
            << "  " << values[c++] << std::endl;
      }
    return out;
  }

  template <class Index, class Value>
  std::ostream &
  outputForOctave(std::ostream &               out,
                  CooData<Index, Value> const &coo,
                  int                          rows,
                  int                          cols)
  {
    auto const &values  = coo.second;
    auto const &indices = coo.first;
    Index       c       = 0;
    for (auto i : indices)
      {
        // Matlab/octave matrices start from 1
        out << i[0] + 1 << "  " << i[1] + 1 << "  " << values[c++] << std::endl;
      }
    out << rows << "  " << cols << "  " << Value(0) << std::endl;
    return out;
  }

  // Version for EigenMAtrices
  template <class EMat>
  std::ostream &
  outputForOctave(std::ostream &out, EMat const &m)
  {
    outputForOctave(out, extractCooData(m), m.rows(), m.cols());
    return out;
  }


  template <class Index, class Value>
  std::ostream &
  operator<<(std::ostream &out, CooData<Index, Value> const &coo)
  {
    return printCooData(out, coo);
  }

  //! COmpute num rows and columns
  /*!
    The coo structure is a stupid structure. It does not stores nrows and cols
    We assume it is the maximum value (+1) of row ad cols indices.

    \param cooMat The matrix value in the COO structure
    \return A pir with num rows and num cols.
  */

  template <class Index, class Value>
  std::pair<Index, Index>
  getRowsAndCols(CooData<Index, Value> const &cooMat)
  {
    std::pair<Index, Index> res{0, 0};
    for (auto const &i : cooMat.first)
      {
        res.first  = std::max(res.first, i[0]);
        res.second = std::max(res.second, i[1]);
      }
    if (not cooMat.first.empty())
      {
        // Add one
        res.first++;
        res.second++;
      }
    return res;
  }

  template <class SparseEigenMatrix, class Index, class Value>
  SparseEigenMatrix
  make_EigenMatrix(CooData<Index, Value> const &cooMat)
  {
    // To be sure we need to check consistency of the types stored in the
    // two stuctures
    using EigIndex = typename SparseEigenMatrix::Index;
    using EigValue = typename SparseEigenMatrix::Scalar;
    static_assert(std::is_convertible<Index, EigIndex>::value,
                  "Index type in the coo structure is not convertible to the "
                  "type used by Eigen");

    static_assert(std::is_convertible<Value, EigValue>::value,
                  "Value type in the coo structure is not convertible to the "
                  "type used by Eigen");

    // I create the triplets
    std::vector<Eigen::Triplet<double>> tripletList;
    tripletList.reserve(cooMat.size());
    // to make life simpler
    auto const &values  = cooMat.second;
    auto const &indices = cooMat.first;

    Index c = 0;

    // Create triplets
    // Note the conversion. Necessary in the case index and values in the coo
    // structure are only convertible to those of eigen and the constructor of
    // Triplet is explicit If the Triplet constructor is not explicit this is
    // not nececcary since the conversion will be implicitely made. An you can
    // just write tripletList.emplace_back(i[0],i[1],values[c++]);
    //
    for (auto i : indices)
      tripletList.emplace_back(EigIndex(i[0]),
                               EigIndex(i[1]),
                               EigValue(values[c++]));

    // Create matrix
    Index nRows;
    Index nCols;

    // A nice use of tie..... Unpack a pair.
    std::tie(nRows, nCols) = getRowsAndCols(cooMat);

    SparseEigenMatrix mat(nRows, nCols);

    mat.setFromTriplets(tripletList.begin(), tripletList.end());

    return mat;
  }

} // namespace LinearAlgebra

#endif
