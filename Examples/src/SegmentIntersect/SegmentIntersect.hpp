#ifndef HPP_SEGMENTINTERSECT_HPP
#define HPP_SEGMENTINTERSECT_HPP
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <concepts>
namespace apsc::Geometry::Intersection {

//! Some helper functions in an anonimous namespace
namespace {
inline double dot(std::array<double, 2> const &a,
                  std::array<double, 2> const &b) {
  return a[0] * b[0] + a[1] * b[1];
}
inline double norm(std::array<double, 2> const &a) {
  return std::sqrt(dot(a, a));
};

inline std::array<double, 2> operator+(std::array<double, 2> const &a,
                                       std::array<double, 2> const &b) {
  return {{a[0] + b[0], a[1] + b[1]}};
}
inline std::array<double, 2> operator-(std::array<double, 2> const &a,
                                       std::array<double, 2> const &b) {
  return {{a[0] - b[0], a[1] - b[1]}};
}
inline std::array<double, 2> operator*(double const &a,
                                       std::array<double, 2> const &b) {
  return {{a * b[0], a * b[1]}};
}
//! Solves Ax=b when A is 2X2
/*! @param tol Tolerance on the determinant
 * @param A the matrix
 * @param b the rhs
 */
inline
std::pair<bool,std::array<double, 2>>
solve(std::array<std::array<double, 2>, 2> const &A, std::array<double, 2> b,
      double const &tol = 1.e-8) {
  auto D = A[0][0] * A[1][1] - A[1][0] * A[0][1];
  if (std::abs(D) <= tol)
    return std::make_pair(false, std::array<double, 2>{0, 0});
  D = 1. / D;
  std::array<double, 2> res;
  res[0] = D * (A[1][1] * b[0] - A[0][1] * b[1]);
  res[1] = D * (A[0][0] * b[1] - A[1][0] * b[0]);
  return {true, res};
}
} // namespace
/*!
 * Expresses the basic semantic of a 2D edge
 * @tparam E the edge
 */
template<typename E>
concept Edge = requires (E e)
{
  {e[0]}->std::convertible_to<std::array<double,2> >;
};

//! A simple struct that contains the result of the intersection test
/*!
  To be able to treat the most general case each segment is allowed
  to have up to two intersections. It happens if the segments overlaps

  @todo It can be bettered by adding another attribute that indicates,
  in the case of two edges end which coincides the relative position on the
  edge. It requires a simple modification of the function segmentIntersect
 */
struct IntersectionStatus {
  //! Segments intersects
  /*!
    True is there is any intersection
   */
  bool intersect = false;
  //! Number of intersections (max 2)
  unsigned int numberOfIntersections = 0u;
  //! Intersection points coordinates
  std::array<std::array<double, 2>, 2> intersectionPoint{{{0., 0.}, {0., 0.}}};
  /*! Intersection points local coordinates
   *
   * For each intersection it provides the local coordinates t0 and t1 so that
   * for each segment i \f$ X_{inter}=X_0 + t_i(X_1-X_0) \f$
   */
  std::array<std::array<double, 2>, 2> intersection_tcoor{{{0., 0.}, {0., 0.}}};
  //! Edges are parallel
  bool parallel = false;
  //! Edges are identical
  bool identical = false;
  /*! Intersection may be end point

    endPointIsIntersection[i][j]=true
    then end j of edge i is at the intersection
  */
  std::array<std::array<bool, 2>, 2> endPointIsIntersection{
      {{false, false}, {false, false}}};

  /*! EdgeS join at the end.
    In that case endPointIsIntersection will be true and the corresponding
    entry will indicate the numering of the end of the other edge.
    -1 indicates that the end is not joined. So
    if endPointIsIntersection[i][j]=true we have
    otherEdgePoint[i][j]=-1 //End point is not joined with the other edge
    otherEdgePoint[i][j]= k //End point j of edge j is joined with end point k
    of other edge
   */
  std::array<std::array<int, 2>, 2> otherEdgePoint = {{{-1, -1}, {-1, -1}}};
  //! Edges are collinear (and thus also parallel)
  bool collinear = false;
  //! Something is not ok
  bool good = true;
  //! Distance, makes sense only if parallel=true
  double distance = 0.0;
};

/*!
  @brief Computes intersection between edges

  It handles also the case of intersection at the segment ends
  @note  It is not so robust because it uses the tolerances in a
  different way: the tolerance tol to test the parametric coordinate
  along the edge line and a scaled tolerance to check distances.
  Another scaled tolerance is used to test if edges are parallel.

  @pre The Edges must have non null length
  @tparam Edge_t A type with operator[i] that returns an array of 2 doubles
  containing the edge end coordinates
  @param S1 First Edge
  @param S2 Second Edge
  @param tol A tolerance,it should greater than epsilon for doubles
  @return Intersection. A data structure containing the info about the
  intersection
 */
template <Edge Edge_t>
IntersectionStatus segmentIntersect(
    const Edge_t &S1, const Edge_t &S2,
    double const tol = std::sqrt(std::numeric_limits<double>::epsilon())) {
  IntersectionStatus out;
  //[0][0] ->1
  //[0][1] ->2
  //[1][0] ->3
  //[1][1] ->4
  std::array<double, 2> const v1 = S1[1] - S1[0];
  std::array<double, 2> const v2 = S2[1] - S2[0];
  auto const len1 = norm(v1);
  auto const len2 = norm(v2);
  // Tolerance for distances
  double tol_dist = tol * std::max(len1, len2);
  // I need to scale the tolerance to check
  // parallelism correctly
  auto tol_sys = 2.0 * tol * len1 * len1 * len2 * len2;
  // First check if segments meet at the end
  for (auto i = 0u; i < 2u; ++i) {
    auto const &P1 = S1[i];
    for (auto j = 0u; j < 2u; ++j) {
      auto const &P2 = S2[j];
      auto dist = norm(P1 - P2);
      if (dist <= tol_dist) {
#ifndef NDEBUG
        if (out.numberOfIntersections >= 2) {
          std::cerr << "Something wrong, cannot have more that 2 intersections"
                    << std::endl;
          out.good = false;
          return out;
        }
#endif
        out.intersect = true;
        out.intersectionPoint[out.numberOfIntersections] = P1;
        out.intersection_tcoor[out.numberOfIntersections] = {
            {double(i), double(j)}};
        ++out.numberOfIntersections;
        out.endPointIsIntersection[0][i] = true;
        out.endPointIsIntersection[1][j] = true;
        out.otherEdgePoint[0][i] = j;
        out.otherEdgePoint[1][j] = i;
      }
    }
  }
  // Handle the case where the two edges are identical!
  if (out.numberOfIntersections == 2u) {
    out.identical = true;
    out.parallel = true;
    out.collinear = true;
    return out;
  }
  // Now solve the linear system that returns the
  // parametric coordinates of the intersection
  std::array<std::array<double, 2>, 2> A;
  // to make life easier I call A and B the
  // ends of the two segmensts
  auto const &A1 = S1[0];
  auto const &B1 = S1[1];
  auto const &A2 = S2[0];
  auto const &B2 = S2[1];
  auto V1 = B1 - A1;
  auto V2 = B2 - A2;
  A[0][0] = dot(V1, V1);
  A[0][1] = -dot(V1, V2);
  A[1][0] = A[0][1];
  A[1][1] = dot(V2, V2);
  std::array<double, 2> b;
  b[0] = dot(A2 - A1, V1);
  b[1] = dot(A1 - A2, V2);
  // find parametric coordinate of the intersection
  auto result = solve(A, b, tol_sys);
  // Standard case, the two lines are not parallel
  if (result.first == true) {
    auto const &t = result.second;
    // Make a stupid check (only in debugging phase)
#ifndef NDEBUG
    const std::array<double, 2> P1 = A1 + t[0] * (B1 - A1);
    const std::array<double, 2> P2 = A2 + t[1] * (B2 - A2);
    if (norm(P1 - P2) > tol_dist)
      std::cerr << " Something strange, intersection points not coincident. "
                   "Distance= "
                << norm(P1 - P2);
#endif
    // The two lines intersect.
    // Check whether we are inside the segments
    double const &t1 = result.second[0];
    double const &t2 = result.second[1];
    // double tol1=tol/len1;
    // double tol2=tol/len2;
    bool inside = (t1 >= -0.5 * tol) && (t1 <= 1.0 + 0.5 * tol) &&
                  (t2 >= -0.5 * tol) && (t2 <= 1.0 + 0.5 * tol);
    if (!inside) {
      // No intersection, end here
      return out;
    } else {
      out.intersect = true;
      // I coud have used the mean
      if (std::abs(t1) <= tol) {
        if (out.endPointIsIntersection[0][0]) {
          // already found. End here
          return out;
        } else {
          out.endPointIsIntersection[0][0] = true;
        }
      }
      if (std::abs(t1 - 1.0) <= tol) {
        if (out.endPointIsIntersection[0][1]) {
          // already found. End here
          return out;
        } else {
          out.endPointIsIntersection[0][1] = true;
        }
      }
      if (std::abs(t2) <= tol) {
        if (out.endPointIsIntersection[1][0]) {
          // already found. End here
          return out;
        } else {
          out.endPointIsIntersection[1][0] = true;
        }
      }
      if (std::abs(t2 - 1.0) <= tol) {
        if (out.endPointIsIntersection[1][1]) {
          // already found. End here
          return out;
        } else {
          out.endPointIsIntersection[1][1] = true;
        }
      }
      out.intersectionPoint[out.numberOfIntersections] = A1 + t1 * (B1 - A1);
      out.intersection_tcoor[out.numberOfIntersections++] = {{t1, t2}};
      return out;
    }
  } else {
    out.parallel = true;
    // Compute distance between the two lines
    /*
      (x-s1(0,0))(s1(1,1)-s1(0,1))-(s1(1,0)*s1(0,0))*(y-s1(0,1))=0
      (s1(1,1)-s1(0,1))x+(s1(0,0)*s1(1,0))y+s1(0,1)(s1(1,0)*s1(0,0))-
      (s1(1,1)-s1(0,1))s1(0,0)=0
    */
    double factor = dot(B1 - A1, A2 - A1) / (len1 * len1);
    auto distVect = (A1 - A2) + factor * (B1 - A1);
    double distance = norm(distVect);
    out.distance = distance;
    // Maybe here you want to use a different tolerance?
    if (distance > tol_dist) {
      // The two segments are separated
      // No intersection.
      return out;
    } else {
      out.collinear = true;
      double t;
      // Segments are collinear!
      // We need to consider all the cases!
      // IS A2 on S1?
      // Maybe I have already considered it earlier
      if (!out.endPointIsIntersection[1][0]) {
        t = dot((A2 - A1), (B1 - A1)) / (len1 * len1);
        if (t >= -0.5 * tol && t <= 1 + 0.5 * tol) {
          out.intersect = true;
          out.intersectionPoint[out.numberOfIntersections] = A2;
          out.intersection_tcoor[out.numberOfIntersections++] = {{t, 0.}};
          out.endPointIsIntersection[1][0] = true;
          if (out.numberOfIntersections == 2)
            return out;
        }
      }
      // IS B2 on S1?
      // Maybe I have already considered it earlier
      if (!out.endPointIsIntersection[1][1]) {
        t = dot((B2 - A1), (B1 - A1)) / (len1 * len1);
        if (t >= -0.5 * tol && t <= 1 + 0.5 * tol) {
          out.intersect = true;
          out.intersectionPoint[out.numberOfIntersections] = B2;
          out.intersection_tcoor[out.numberOfIntersections++] = {{t, 1}};
          out.endPointIsIntersection[1][1] = true;
          if (out.numberOfIntersections == 2)
            return out;
        }
      }
      // IS A1 on S2?
      // Maybe I have already considered it earlier
      if (!out.endPointIsIntersection[0][0]) {
        t = dot((A1 - A2), (B2 - A2)) / (len2 * len2);
        if (t >= -0.5 * tol && t <= 1 + 0.5 * tol) {
          out.intersect = true;
          out.intersectionPoint[out.numberOfIntersections] = A1;
          out.intersection_tcoor[out.numberOfIntersections++] = {{0, t}};
          out.endPointIsIntersection[0][0] = true;
          if (out.numberOfIntersections == 2)
            return out;
        }
      }
      // IS B1 on S2?
      // Maybe I have already considered it earlier
      if (!out.endPointIsIntersection[0][1]) {
        t = dot((B1 - A2), (B2 - A2)) / (len2 * len2);
        if (t >= -0.5 * tol && t <= 1 + 0.5 * tol) {
          out.intersect = true;
          out.intersectionPoint[out.numberOfIntersections] = B1;
          out.intersection_tcoor[out.numberOfIntersections++] = {{1., t}};
          out.endPointIsIntersection[0][1] = true;
          if (out.numberOfIntersections == 2)
            return out;
        }
      }
    }
  }
  return out;
}
} // namespace apsc::Geometry::Intersection
std::ostream &
operator<<(std::ostream &,
           apsc::Geometry::Intersection::IntersectionStatus const &);
#endif
