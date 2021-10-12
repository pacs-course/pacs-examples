/*
 * MeshTria.h
 *
 *  Created on: Oct 3, 2010
 *      Author: forma
 */

#ifndef MESHTRIA_H_
#define MESHTRIA_H_
#include "femMesh.hpp"
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>
namespace Fem
{
class MeshTria;

//! Exposes Mesh class implementation
/*!
 * Used to allow mesh readers to access private mesh
 * data.
 */
struct MeshHandler
{
  MeshHandler(MeshTria &mesh);
  std::vector<Point> &   pointList;
  std::vector<Triangle> &elementList;
  std::vector<Edge> &    edgeList;
  std::vector<Edge> &    bEdgeList;
  MeshTria &             m;
};

//! Base class for all readers
class MeshReader
{
public:
  MeshReader(bool verbose = false) : M_verbose(verbose){};
  virtual ~MeshReader(){};
  //! Reads a mesh
  virtual int read(MeshTria &mesh, std::string const &filename) = 0;
  //! set verbosity
  void
  setverbose(bool set = true)
  {
    M_verbose = set;
  };

protected:
  bool M_verbose;
};

class MeshTria
{
public:
  typedef std::size_t size_type;
  //! Constructor
  MeshTria() = default;
  //! Reads a mesh form a file using a reader
  MeshTria(std::string const filename, MeshReader &);
  MeshTria(const MeshTria &);
  MeshTria(MeshTria &&) = default;
  //! Assignement operators
  MeshTria &operator=(MeshTria const &);
  MeshTria &operator=(MeshTria &&) = default;

  friend struct MeshHandler;
  //! Number of points
  size_type
  num_points() const
  {
    return M_pointList.size();
  }
  //! ith point
  Point const &
  point(size_type i) const
  {
    return M_pointList[i];
  }
  //! Number of elements
  size_type
  num_elements() const
  {
    return M_elementList.size();
  }
  //! ith element
  Triangle const &
  element(size_type i) const
  {
    return M_elementList[i];
  }
  //! Ith element with specific name
  Triangle const &
  triangle(size_type i) const
  {
    return M_elementList[i];
  }
  //! Number of edges
  size_type
  num_edges() const
  {
    return M_edgeList.size();
  }
  //! ith Edge
  Edge const &
  edge(size_type i) const
  {
    return M_edgeList[i];
  }
  //! Are edges stored?
  bool
  has_Edges() const
  {
    return !M_edgeList.empty();
  }
  //! Number of boundary edges
  size_type
  num_bEdges() const
  {
    return M_bEdgeList.size();
  }
  //! ith edge
  Edge const &
  bEdge(size_type i) const
  {
    return M_bEdgeList[i];
  }
  //! Are boundary edges stored
  bool
  has_bEdges() const
  {
    return !M_bEdgeList.empty();
  }
  //! Read a mesh from file using a reader
  int readMesh(std::string const &file, MeshReader &);
  //! measure of the domain
  double measure() const;
  //! Test mesh consistency
  bool checkmesh() const;

private:
  std::vector<Point>    M_pointList;
  std::vector<Triangle> M_elementList;
  std::vector<Edge>     M_edgeList;
  std::vector<Edge>     M_bEdgeList;
};

std::ostream &operator<<(std::ostream &, MeshTria const &);
} // namespace Fem

#endif /* MESHTRIA_H_ */
