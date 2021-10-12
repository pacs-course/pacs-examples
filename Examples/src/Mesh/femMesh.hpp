/*
 * femMesh.hpp
 *
 *  Created on: Oct 1, 2010
 *      Author: forma
 */

#ifndef FEMMESH_HPP_
#define FEMMESH_HPP_
#include "Eigen/Dense"
#include "geo.hpp"
#include "smallMatrix.hpp"
#include <iosfwd>
#include <limits>
namespace Fem
{
using Id = unsigned int;
using BcId = unsigned int;
using Eigen::Matrix;
using Geometry::ndim;
using LinearAlgebra::SMatrix;
using GeoPoint = Geometry::Point;

//! Indicates a non valid number for an identifier
const unsigned int NVAL = std::numeric_limits<unsigned int>::max();

//! Extra features for a geometric entity used in a fem code
/*!
 * This class implements the handling of boundary/domain markers
 * and entity identifiers. If the entity is stored in a linear container
 * the identifier should be equal to the position of the object
 * in the container.
 */
class FemEntity
{
public:
  FemEntity(Id id = NVAL) : M_id(id), M_bcId(id) {}
  FemEntity(const FemEntity &) = default;

  bool
  unassignedId() const
  {
    return M_id == NVAL;
  }
  bool
  unassignedBc() const
  {
    return M_bcId == NVAL;
  }
  Id &
  id()
  {
    return M_id;
  }
  Id const &
  id() const
  {
    return M_id;
  }
  BcId &
  bcId()
  {
    return M_bcId;
  }
  BcId const &
  bcId() const
  {
    return M_bcId;
  }

protected:
  Id   M_id;
  BcId M_bcId;
};

class Point : public GeoPoint, public FemEntity
{
public:
  explicit Point(double x = 0, double y = 0) : GeoPoint(x, y), FemEntity(){};
  Point(GeoPoint const &p) : GeoPoint(p), FemEntity(){};
  Point(const Point &) = default;
  Point(Point &&) = default;
  Point &operator=(const Point &p) = default;
  Point &operator=(Point &&p) = default;
};

//! A FEM edge
/*!
  /todo composition with Geometry::edge
 */
class Edge : public FemEntity
{
public:
  static const int numVertices = 2;
  static const int numSides = 1;
  static const int myDim = 1;
  //! Constructs an empty edge
  Edge() : M_points{{nullptr, nullptr}} {};
  //! Points are given (by reference)
  Edge(Point &a, Point &b) : M_points{{&a, &b}} {};
  void changePoint(int i, Point &);
  //! We get the points by operator [] (defined in-class for inlining)
  Point const &
  operator[](int i) const
  {
    return *(M_points[i]);
  }
  Point &
  operator[](int i)
  {
    return *(M_points[i]);
  }
  //! Edge Length
  double measure() const;
  //! Returns x=x(t)
  GeoPoint map(double const &) const;
  //! dx/dt at a point t\in(0,1) returns edge length
  Eigen::Matrix<double, ndim, 1> jacobian(double const &) const;
  //! checks if the edge is empty: M_points contain null pointers
  bool
  empty() const
  {
    return M_points[0] == nullptr || M_points[1] == nullptr;
  } // inlined function
private:
  std::array<Point *, numVertices> M_points;
};

//! Class for a triangular, geometrically linear finite element.
/*!
 */
class Triangle : public Geometry::Triangle<Fem::Point>, public FemEntity
{
public:
  using GeoTriangle = Geometry::Triangle<Fem::Point>;
  static const int numVertices = 3;
  static const int numSides = 3;
  static const int myDim = 2;
  using size_type = std::size_t;
  using Point = Fem::Point; // Not needed but clearer!
  /*!\defgroup Constructors Constructors and assignement ops
    @{
  */
  //! Constructs an empty triangle
  Triangle();
  //! Triangle by points. Id not compulsory
  Triangle(Point &a, Point &b, Point &c, Id id = NVAL);
  /*!
    If a copy a triangle I copy the pointers to Point.
    So I have a triangle with the same points
  */
  Triangle(const Triangle &) = default;
  //! If I assign a triangle I copy the pointers to Point
  /*! Since the semantic is ambiguous I keep it private.
   */
  Triangle &operator=(const Triangle &) = default;
  //! A triangle can be moved. No risk here
  Triangle(Triangle &&) = default;
  //! Move copy
  Triangle &operator=(Triangle &&) = default;
  /*!@}*/
  //! Changing Points
  /*!
    I need to hide that on Geometry::Triangle
    since I need to recompute the Jacobian
   */
  void changePoint(int i, Point &p);
  //! Cloning a triangle
  /*!
    Creates a new triangle with the same gometrical points
    represented by different objects
   */

  //! Return x=T(u,v)
  GeoPoint
  map(GeoPoint const &u) const
  {
    Triangle const &t = *this;
    return GeoPoint(t[0][0] + M_J(0, 0) * u[0] + M_J(0, 1) * u[1],
                    t[0][1] + M_J(1, 0) * u[0] + M_J(1, 1) * u[1]);
  }
  //! Return \f$(u,v) = T^{-1}(x)$
  GeoPoint invMap(GeoPoint const &x) const;
  //! The area
  /* It hides the same method in the base class, since it is more efficient.
   */
  double
  measure() const
  {
    return 0.5 * detJ;
  };
  //\f$J=dx/d(u,v)\f$ at a point in the ref plane return det(J)
  Eigen::Matrix<double, myDim, ndim> jacobian(double u = 0, double v = 0) const;
  //\f$(d(xy)/d(uv))^{-1}\f$ at a point in the ref plane
  // \f$J_{ij}=dx_i/du_j\f$
  Eigen::Matrix<double, myDim, ndim> invJac(double u = 0, double v = 0) const;
  bool                               empty() const;
  // verifies orientation
  bool
  orientation() const
  {
    return detJ > 0.0;
  };
  //! sets orientation
  void setOrientation(bool positive = true);
  //! Mass Matrix (unitary density)
  Eigen::Matrix<double, 3, 3> localMassMatrix() const;
  //! Stiffness matrix (unitary diffusivity)
  Eigen::Matrix<double, 3, 3> localStiffMatrix() const;

private:
  // I don't store directly a eigen matrix because of the limitations
  // of the current problems of alignement (see eigen 3.0 documentation)
  // It is not very efficient and should be changed asap
  SMatrix<double, myDim, ndim> M_J;
  SMatrix<double, myDim, ndim> M_invJ;
  double                       detJ;
  void                         M_setJac();
};
std::ostream &operator<<(std::ostream &, Triangle const &);

// Edge comparision operators

} // namespace Fem

#endif /* FEMMESH_HPP_ */
