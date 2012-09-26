/*!
  \file shape.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \author Tiziano Passerini <tiziano.passerini@polimi.it>
  \date 11-6-2005
*/

#ifndef SHAPE_HPP
#define SHAPE_HPP 1

#define DIM 2

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <iostream>
#include <vector>

namespace Geometry {
  namespace UBLAS = boost::numeric::ublas;
  //! The floating point type
  typedef double Real;

  /*! 
    \class Point
    \brief Class for geometric points
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-10-2005
  */
  class Point : public UBLAS::bounded_vector<Real, DIM> {
  public:
    /** @name Constructors
     */
    //@{
    //! Default constructor
    Point() : M_ID(-1) {}
    //! Constructor taking the identifier
    Point(int ID) : M_ID(ID) {}
    //@}
    /** @name Methods
     */
    //@{
    //! Return the identifier
    inline int& id() { return M_ID; }
    inline int const& id() const { return M_ID; }
    //@}
    //! Returns wether the point is empty
    inline bool empty() const {return M_ID==-1;};
    
    
    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream& ostr, const Point& P);
    //@}
  private:
    //! The point identifier
    int M_ID;
  };

  class Edge; 

  /*!
    \class Shape
    \brief Base class for geometric shapes
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-6-2005
  */

  class Shape {
  public:
    /** @name Constructors and destructors
     */
    //@{
    Shape(int nP, int nE=0);
    Shape(const Shape& s);
    virtual ~Shape() {}
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of points
    inline int nP() const { return M_nP; }
    //! Return the number of edges
    inline int nE() const { return M_nE; }
    //! Return the \f$i^{\mathrm{th}}\f$ point
    inline Point& point(int i) { return M_points[i]; }
    //! Return the \f$i^{\mathrm{th}}\f$ point (read-only version)
    inline const Point& point(int i) const { return M_points[i]; }
    //! Return the \f$i^{\mathrm{th}}\f$ edge
    virtual Edge edge(int i) = 0;
    //! Return the \f$i^{\mathrm{th}}\f$ point (read-only version)
//    virtual const Edge& edge(int i) const = 0;
    //@}
    /** @name Members
     */
    //@{
    virtual Real measure() = 0;
    //@}
    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream& ostr, const Shape& s);
    //@}
  private:
    //! The number of points
    int M_nP;
    //! The number of edges
    int M_nE;
    //! The array of points
    std::vector<Point> M_points;
  };

  /*!
    \class Edge
    \brief A class to store edges
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-6-2005
  */
  class Edge : public Shape {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor taking the ID of two points
    Edge(Point P0, Point P1) : Shape(2) {
      this->point(0) = P0;
      this->point(1) = P1;
    }
    //! Constructor for an empty edge
    Edge():Shape(2)
    {
      this->point(0) = Point();
      this->point(1) = Point();
    }

    //! Tells that the edge is empty

    inline bool empty() const {return  this->point(0).empty();}
    
    
    //@}
    /** @name Members
     */
    //@{
    inline Real measure() { return norm_2(this->point(1)
					  - this->point(0)); };
    virtual inline Edge edge(int i) {
			std::cout << "\nWARNING: class Edge has no edge entities" << std::endl;
			return Edge();}
    //@}
  };

 inline  bool operator==(const Edge& e1, const Edge& e2) {
   return e1.point(0).id()== e2.point(0).id()&&e1.point(2).id()== e2.point(2).id();
  }
}

#endif
