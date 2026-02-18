#ifndef _HH_MESH_HH
#define _HH_MESH_HH
#include "domain.hpp"
#include "meshGenerators.hpp"
#include <cstddef>
#include <vector>
namespace Geometry
{
class Mesh1D
{
public:
  //! Default constructor is defaulted.
  Mesh1D() = default;
  //! Constructor for an equaly spaced mesh
  /*!
    \param d  A domain
    \param n  Number of intervals (not nodes!)
  */
  Mesh1D(Domain1D const &d, std::size_t n);
  //! Constructor for an variably spaced mesh
  /*!
    \param gf the policy for generating mesh
  */
  explicit Mesh1D(Geometry::OneDMeshGenerator const &gf)
    : myDomain{gf.getDomain()}, myNodes{gf()} {};
  //! Generate mesh (it will destroy old mesh)
  /*!
    @param mg a mesh generator
   */
  void reset(OneDMeshGenerator const &mg);

  //! Number of nodes.
  [[nodiscard]] std::size_t
  numNodes() const noexcept
  {
    return myNodes.size();
  }
  //! The i-th node.
  [[nodiscard]] double
  operator[](std::size_t i) const
  {
    return myNodes[i];
  }
  [[nodiscard]] double at(std::size_t i) const { return myNodes.at(i); }
  //! To use the mesh in range based for loop I need begin()
  std::vector<double>::iterator
  begin()
  {
    return myNodes.begin();
  }
  std::vector<double>::const_iterator
  begin() const
  {
    return myNodes.begin();
  }
  std::vector<double>::const_iterator
  cbegin() const
  {
    return myNodes.cbegin();
  }
  //! To use the mesh in range based for loop I need end()
  std::vector<double>::iterator
  end()
  {
    return myNodes.end();
  }
  std::vector<double>::const_iterator
  end() const
  {
    return myNodes.end();
  }
  std::vector<double>::const_iterator
  cend() const
  {
    return myNodes.cend();
  }
  //! Return the domain associated with this mesh.
  /*!
    The mesh generator may store state, so this provides direct access.
  */
  [[nodiscard]] Domain1D const &
  domain() const noexcept
  {
    return myDomain;
  }
  //! The minimum mesh size (distance between consecutive nodes).
  [[nodiscard]] double hmin() const;
  //! The maximum mesh size (distance between consecutive nodes).
  [[nodiscard]] double hmax() const;

private:
  Domain1D            myDomain;
  std::vector<double> myNodes;
};

} // namespace Geometry
#endif
