#ifndef MESH_HPP
#define MESH_HPP 1

#include <fstream>
#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "shape.hpp"
#include "triangle.hpp"
#include "quadrangle.hpp"

namespace Geometry {
  class Mesh {
  public:
    /** @name Typedefs
     */
    //@{
    typedef Edge<2> Edge;
    typedef Point<2> Point;
    typedef Shape<2> Shape;
    typedef boost::shared_ptr<Shape> element_pointer_type;
    typedef std::vector<Point> point_vector_type;
    typedef std::vector<element_pointer_type> element_vector_type;
    //@}
    /** @name Constructors
     */
    //@{
    //! Read mesh from file 
    Mesh(char* fileName);
    //! Destroy dynamically created elements
    ~Mesh(){};
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of points
    int nP() const { return M_points.size(); }
    //! Return the number of elements
    int nE() const { return M_elements.size(); }
    //! Return a reference to the \f$i^{\mathrm{th}}\f$ point
    const Point& point(int i) const { return M_points[i]; }
    //! Return a reference to the \f$i^{\mathrm{th}}\f$ element
    const Shape& element(int i) const { return *M_elements[i]; }
    //@}
    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream& ostr, const Mesh& m);
    //@}
  private:
    //! Array of points
    point_vector_type M_points;
    //! Array of elements
    element_vector_type M_elements;
  };
}

#endif
