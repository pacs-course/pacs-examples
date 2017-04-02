/*!
  \file shape.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \author Tiziano Passerini <tiziano.passerini@polimi.it>
  \date 11-6-2005
*/

#ifndef SHAPE_HPP
#define SHAPE_HPP 1

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#define DIM 2
#define NOTSET std::numeric_limits<int>::max()

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
    Point() : M_ID(0) {}
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
    inline Point& point(const int& i) { return M_points[i]; }
    //! Return the \f$i^{\mathrm{th}}\f$ point (read-only version)
    inline const Point& point(const int& i) const { return M_points[i]; }
    //! Return the \f$i^{\mathrm{th}}\f$ edge
    virtual Edge edge(const int& i) = 0;   
    //! Return the \f$i^{\mathrm{th}}\f$ point (read-only version)
//    virtual const Edge& edge(int i) const = 0;
    //@}
    /** @name Members
     */
    //@{
    virtual Real measure() const = 0;
    //@}
    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream& ostr, const Shape& s);
    //@}
  private:
    //! The number of points
    int M_nP;
    //! The array of points
    std::vector<Point> M_points;
    //! The number of edges
    int M_nE;
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
    Edge(Point P0, Point P1);
    //@}
    /** @name Members
     */
    //@{
    inline Real measure() const
    { 
      return norm_2(this->point(1) - this->point(0)); 
    };

    virtual inline Edge edge(const int&) 
    {
      std::cout << "\nWARNING: class Edge has no edge entities" << std::endl; 
      return *this;
    } 

    inline void setElemLeft( const int& elemLeft) { M_elemLeft = elemLeft; }

    inline const int elemLeft() const { return M_elemLeft; }

    inline void setElemRight( const int& elemRight) { M_elemRight = elemRight; }

    inline const int elemRight() const { return M_elemRight; }
   
    inline bool isBoundary() const
    {
      return ( M_elemLeft == NOTSET ) || ( M_elemRight == NOTSET );
    }

    inline bool isSet() const
    {
      return ( M_elemLeft == NOTSET ) && ( M_elemRight == NOTSET );
    }
    //@}

    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream& ostr, const Edge& s);
    //@}

  private:
    // The id of the left element
    int M_elemLeft;
    // The id of the right element
    int M_elemRight;
  };

  /*
 inline bool operator<(const Edge& e1, const Edge& e2) {
   if( e1.point(0).id() == e2.point(0).id() )
     return( e1.point(1).id() < e2.point(1).id() );
   return ( e1.point(0).id() < e2.point(0).id() );
}
  */
 inline bool operator==(const Edge& e1, const Edge& e2) {
   return ( ( norm_2(e1.point(0) - e2.point(0)) == 0. &&
	      norm_2(e1.point(1) - e2.point(1)) == 0. ) ||
	    ( norm_2(e1.point(1) - e2.point(0)) == 0. &&
	      norm_2(e1.point(0) - e2.point(1)) == 0.) );
 } 
 
  
inline bool operator<(const Edge& e1, const Edge& e2) {
  unsigned int i1=std::max( e1.point(0).id(),e1.point(1).id());
  unsigned int i2=e1.point(0).id()+e1.point(1).id()-i1;
  unsigned int j1=std::max( e2.point(0).id(),e2.point(1).id());
  unsigned int j2=e2.point(0).id()+e2.point(1).id()-j1;
   if( i1==j1)
     return i2 < j2 ;
   return  i1 < j1 ;
}
 
}

#endif
