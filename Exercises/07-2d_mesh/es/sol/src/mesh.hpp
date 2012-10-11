#ifndef MESH_HPP
#define MESH_HPP 1

#include <fstream>
#include <string>
#include <vector>

#include <map>
#include <set>
#include <algorithm>
#include <functional>

#include "boost/shared_ptr.hpp"

#include "shape.hpp"
#include "triangle.hpp"
#include "quadrangle.hpp"


//! Function to print the value of a map element p
void print( std::pair<int,std::string> p );

//! Find all map elements whose value is equal to V
template <class K, class V>
class value_equals {
  private:
    V value;
  public:
    // constructor (initialize value to compare with)
    value_equals (const V& v)
     : value(v) {
    }
    // comparison
    bool operator() (std::pair<const K, V> elem) const {
        return elem.second == value;
    }
};

namespace Geometry {
	
  //! Enum to be used in Mesh class to associate string values to integer values
  enum BoundaryStringValue { Dirichlet, Neumann, Internal };

  /*! 
    \class Mesh
    \brief Simple class for mesh handling
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \author Tiziano Passerini <tiziano.passerini@polimi.it>
    \date 01-2008
  */
  class Mesh {
  public:
    /** @name Typedefs
     */
    //@{
    typedef boost::shared_ptr<Shape> element_pointer_type;
    typedef std::vector<Point> point_vector_type;
    typedef std::set<Edge> edge_set_type;
    typedef std::map<int, std::string> boundary_point_map_type;
    typedef std::multimap<std::string, int> boundary_point_multimap_type;
    typedef std::vector<element_pointer_type> element_vector_type;
    typedef std::multimap<int, element_pointer_type> patch_multimap_type;
    //@}
        /** @name Constructors
     */
    //@{
    //! Read mesh from file 
    Mesh(const char* fileName);
    //! Destroy dynamically created elements
    ~Mesh(){};
    //@}
    /** @name Members
     */
    //@{
    //! Build the sets of edges
    void buildEdges();
    //! Prints to the screen a list of nodes having bcname boundary conditions
    void bc( std::string const& bcname );
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of points
    inline int nP() const { return M_points.size(); }
    //! Return the number of elements
    inline int nE() const { return M_elements.size(); }
    //! Return a pointer to the \f$i^{\mathrm{th}}\f$ point
    inline const Point& point(int i) const { return M_points[i]; }
    //! Return a pointer to the \f$i^{\mathrm{th}}\f$ element
    inline const Shape& element(int i) const { return *M_elements[i]; }
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
    //! Map for boundary points
    boundary_point_map_type M_boundary_points;
    //! Multimap for boundary points
    boundary_point_multimap_type M_boundary_conditions;
    //! Set of edges
    edge_set_type M_edges;
    //! Set of boundary edges
    edge_set_type M_boundary_edges;
    //! Set of Neumann boundary edges
    edge_set_type M_neumann_boundary_edges;
    //! Patches of points
    patch_multimap_type M_patches;
    // other possibility
    // std::vector< std::list<Shape*> > M_patches;

    //! Value-Definitions of the different String values
    std::map<std::string, BoundaryStringValue> M_map_boundary_string_values;

  };
}

#endif
