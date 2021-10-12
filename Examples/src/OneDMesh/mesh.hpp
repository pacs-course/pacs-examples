#ifndef _HH_MESH_HH
#define _HH_MESH_HH
#include "domain.hpp"
#include "meshGenerators.hpp"
#include <functional>
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
  Mesh1D(Domain1D const &d, unsigned int const &n);
  //! Constructor for an variably spaced mesh
  /*!
    \param gf the policy for generating mesh
  */
  Mesh1D(Geometry::OneDMeshGenerator const &gf)
    : myDomain{gf.getDomain()}, myNodes{gf()} {};
  //! Generate mesh (it will destroy old mesh)
  /*!
    @param mg a mesh generator
   */
  void reset(OneDMeshGenerator const &mg);

  //! Number of nodes.
  unsigned int
  numNodes() const
  {
    return myNodes.size();
  }
  //! The i-th node.
  double
  operator[](int i) const
  {
    return myNodes[i];
  }
  //! To use the mesh in range based for loop I need begin()
  std::vector<double>::iterator
  begin()
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
  cend() const
  {
    return myNodes.cend();
  }
  //! I return a copy of the DOmain1D.
  /*!
    In case it is needed.
  */
  Domain1D
  domain() const
  {
    return myDomain;
  }
  //! The max mesh size.
  double hmin() const;
  //! The max mesh size.
  double hmax() const;

private:
  Domain1D            myDomain;
  std::vector<double> myNodes;
};

} // namespace Geometry
#endif
