#ifndef HH_UTILITIES__HPP
#define HH_UTILITIES__HPP
#include "COOExtractor.hpp"

#include <utility>
#include <vector>

#include <Eigen/Core>
namespace LinearAlgebra
{
  //! A structure for the adjacency graph
  /*!
    Only for educational purposes. For any serious programming with graphs
    use the boost graph library!
  */
  class AdjacencyGraph
  {
  public:
    //! I use the same type use by Eigen for the index
    using Index                            = Eigen::Index;
    AdjacencyGraph()                       = default;
    AdjacencyGraph(AdjacencyGraph const &) = default;
    AdjacencyGraph(AdjacencyGraph &&)      = default;
    AdjacencyGraph &
    operator=(AdjacencyGraph const &) = default;
    AdjacencyGraph &
    operator=(AdjacencyGraph &&) = default;

    //! It is constructed from a coo structure.
    //! No other constructor is probably necessary
    template <class I, class Value>
    AdjacencyGraph(CooData<I, Value> const &);
    //! Get all adjacent nodes
    std::vector<Index>
    ad(Index j) const
    {
      return M_graph[j];
    }
    //! Get degree of a node
    Index
    degree(Index j) const
    {
      return M_degree[j];
    }
    //! Write info on the graph
    /*!
      \param out The output stream
      \param verbose. If true it prints bandwidth per row.
     */
    void
    info(std::ostream &out, bool verbose = false) const;
    //! Num Nodes
    unsigned int
    num_nodes() const
    {
      return M_degree.size();
    }

  private:
    //! The graph, represented as a vector of vectors
    std::vector<std::vector<Index>> M_graph;
    //! The degree of each node
    std::vector<Index> M_degree;
  };

  //! A helper function to create adjacency graph directly from any Eigen
  //! sparse matrix expression. Note the use of CRTP, so it can apply
  //! efficiently to any matrix expression, not just a full fledged matrix.
  template <class Derived>
  AdjacencyGraph
  make_Adjacency(Eigen::SparseMatrixBase<Derived> const &EMat)
  {
    return AdjacencyGraph(extractCooData(EMat));
  }

  template <class I, class Value>
  AdjacencyGraph::AdjacencyGraph(CooData<I, Value> const &coo)
  {
    Index nrow;
    // An example of the use of std::ignore. I need only the rows!
    std::tie(nrow, std::ignore) = getRowsAndCols(coo);
    // set deree to 0
    M_degree.resize(nrow, 0);
    M_graph.resize(nrow);
    auto const &indices = coo.first;
    for (auto i : indices)
      {
        M_graph[i[0]].push_back(i[1]);
        M_degree[i[0]]++;
      }
  }

  //! A little helper function to apply symmetric permutation to a Eigen matrix
  /*
    This function is made to avoid remembering how the permutation is made
    in Eigen and also to avoid errors due to the still inconsistent treatment
    of indices type in sparse Eigen matrices and permutation matrices.

    \param EigenM a Eigen matrix type (not necessarily sparse

    \param V the type of array of permited indices. It should implement begin()
    and end() according to stl rules.

    \param EMat The Eigen matrix

    \param v The aray of permuted indices i -> v[i]. It is up to the user to
    verify that they is correct in size and stores values corresponding to a
    permutation. The stored type must be convertible to int.

    \return The permuted matrix, with both row and column exchanged

    Note: we use a rvalue reference to have a universal reference to the vector.
    So the vector con be moved if neeeded to save memory (not so relevant here,
    it is for educational purposes).

   */
  template <class EigenM, class V>
  EigenM
  applySymmetricPermutation(EigenM const &EMat, V &&v)
  {
    // Indices must be converted to an Eigen vector of int.
    // This is a limitation of Eigen
    Eigen::Matrix<int, Eigen::Dynamic, 1> indices(v.size());
    // A counter
    Eigen::Index j(0);
    // coeffRef does not do bounds check. So it is faster.
    // I use static_cast becouse I need int, while the indices may have been
    // stored as unsigned int This is to overcome a limitation of Eigen sparse
    // matrices (I hope they fix it soon)
    for (auto const &i : v)
      indices.coeffRef(j++, 0) = static_cast<int>(i);
    EigenM R;
    R = EMat.twistedBy(indices.asPermutation());
    // Unfortunately doing simply
    // return EMat.twistedBy(indices.asPermutation());
    // without creating the matrix R (and so maybe taking advantage of RVO)
    // is not working because of limitation in the constructors for Eigen
    // matrices
    return R;
  }

  //! Computes Normal or Reverse Cuthill-McKee ordering
  std::vector<LinearAlgebra::AdjacencyGraph::Index>
  applyRCMK(AdjacencyGraph const &g, bool reverse = true);

  //! Applies RCMK algorithm to a eigen matrix
  template <class SpMat>
  SpMat
  computeRCMK(SpMat const &mat, bool reverse = true)
  {
    auto v = applyRCMK(AdjacencyGraph(extractCooData(mat)), reverse);
    return applySymmetricPermutation(mat, v);
  }


} // namespace LinearAlgebra
#endif
