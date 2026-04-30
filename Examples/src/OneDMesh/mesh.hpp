#ifndef _HH_MESH_HH
#define _HH_MESH_HH
/*!
 * @file mesh.hpp
 * @brief Storage class for one-dimensional meshes.
 */
#include "domain.hpp"
#include "meshGenerators.hpp"
#include <cstddef>
#include <vector>
namespace Geometry
{
/*!
 * @brief Concrete one-dimensional mesh made of ordered node coordinates.
 *
 * The class stores the discretized domain together with the vector of node
 * positions. A mesh can be built either directly from a uniform discretization
 * request or from any object implementing the `OneDMeshGenerator` interface.
 */
class Mesh1D
{
public:
  //! Default constructor.
  Mesh1D() = default;
  /*!
   * @brief Construct a uniformly spaced mesh.
   * @param d Domain to be discretized.
   * @param n Number of intervals, not number of nodes.
   */
  Mesh1D(Domain1D const &d, std::size_t n);
  /*!
   * @brief Construct a mesh from a generator policy.
   * @param gf Generator used to compute the mesh nodes.
   */
  explicit Mesh1D(Geometry::OneDMeshGenerator const &gf)
    : myDomain{gf.getDomain()}, myNodes{gf()} {};
  /*!
   * @brief Replace the current mesh with a newly generated one.
   * @param mg Mesh generator to be used.
   */
  void reset(OneDMeshGenerator const &mg);

  //! @brief Number of nodes stored in the mesh.
  [[nodiscard]] std::size_t
  numNodes() const noexcept
  {
    return myNodes.size();
  }
  /*!
   * @brief Unchecked access to the `i`-th node.
   * @param i Node index.
   * @return Coordinate of the selected node.
   */
  [[nodiscard]] double
  operator[](std::size_t i) const
  {
    return myNodes[i];
  }
  /*!
   * @brief Bounds-checked access to the `i`-th node.
   * @param i Node index.
   * @return Coordinate of the selected node.
   * @throw std::out_of_range if `i >= numNodes()`.
   */
  [[nodiscard]] double at(std::size_t i) const { return myNodes.at(i); }
  //! @brief Iterator to the first node.
  std::vector<double>::iterator
  begin()
  {
    return myNodes.begin();
  }
  //! @brief Constant iterator to the first node.
  std::vector<double>::const_iterator
  begin() const
  {
    return myNodes.begin();
  }
  //! @brief Constant iterator to the first node.
  std::vector<double>::const_iterator
  cbegin() const
  {
    return myNodes.cbegin();
  }
  //! @brief Iterator past the last node.
  std::vector<double>::iterator
  end()
  {
    return myNodes.end();
  }
  //! @brief Constant iterator past the last node.
  std::vector<double>::const_iterator
  end() const
  {
    return myNodes.end();
  }
  //! @brief Constant iterator past the last node.
  std::vector<double>::const_iterator
  cend() const
  {
    return myNodes.cend();
  }
  /*!
   * @brief Access the domain associated with the mesh.
   * @return The stored one-dimensional domain.
   */
  [[nodiscard]] Domain1D const &
  domain() const noexcept
  {
    return myDomain;
  }
  /*!
   * @brief Minimum element size.
   * @return Minimum distance between consecutive nodes.
   */
  [[nodiscard]] double hmin() const;
  /*!
   * @brief Maximum element size.
   * @return Maximum distance between consecutive nodes.
   */
  [[nodiscard]] double hmax() const;

private:
  //! Domain represented by the mesh.
  Domain1D            myDomain;
  //! Node coordinates.
  std::vector<double> myNodes;
};

} // namespace Geometry
#endif
