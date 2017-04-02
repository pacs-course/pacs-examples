/*!
  \file shape.hpp
  \author Daniele A. Di Pietro <dipietro@unibg.it>
  \author Tiziano Passerini <tiziano.passerini@polimi.it>
  \date 11-6-2005
*/

#ifndef SHAPE_HPP
#define SHAPE_HPP 1
#include <iostream>
#include <vector>

namespace Geometry {
  //! The floating point type
  typedef double Real;

  /*!
    \class Point
    \brief Class for geometric points
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-10-2005
  */
  template <int DIM=2>
  class Point{
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
    int& id() { return M_ID; }
    int const & id()const { return M_ID; }
    //@}
    //! @name coordinates
    //@{
    double & operator [](int i){return M_P[i];}
    double const & operator [](int i)const {return M_P[i];}
    //@}
    /** @name Output
     */
    //@{
    //! Declared in class because it depends on the template parameter
    friend std::ostream& operator<<(std::ostream& ostr, const Point& P){
      ostr<<"("<<P.M_P[0]<<", "<<P.M_P[1]<<std::endl;
    }
    //@}
  private:
    //! The point identifier
    int M_ID;
    //! The point container
    double M_P[DIM];
  };
  
  /*!
    \class Shape
    \brief Base class for geometric shapes
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-6-2005
  */
  
  template<int DIM>
  class Shape {
  public:
    /** @name Constructors and destructors
     */
    //@{
    Shape(int nP);
    Shape(const Shape& s);
    virtual ~Shape() {}
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of points
    int nP() const { return M_nP; }
    //! Return the number of edges
    int nE() const { return M_nE; }
    //! Return the \f$i^{\mathrm{th}}\f$ point
    Point<DIM>& point(int i) { return M_points[i]; }
    //! Return the \f$i^{\mathrm{th}}\f$ point (read-only version)
    const Point<DIM>& point(int i) const { return M_points[i]; }
    //@}
    /** @name Members
     */
    //@{
    virtual Real measure() = 0;
    //@}
    /** @name Output
     */
    //@{
    template <int DD>
    friend
    std::ostream& operator<<(std::ostream& ostr, const Shape<DD>& s);
    //@}
  private:
    //! The number of points
    int M_nP;
    //! The array of points
    std::vector<Point<DIM> > M_points;
    //! The number of edges
    int M_nE;
  };

  /*!
    \class Edge
    \brief A class to store edges
    \author Daniele A. Di Pietro <dipietro@unibg.it>
    \date 11-6-2005
  */
  template<int DIM>
  class Edge : public Shape<DIM> {
  public:
    typedef Point<DIM> MyPoint;
    /** @name Constructors
     */
    //@{
    //! Constructor taking the ID of two points
    Edge(MyPoint P0, MyPoint P1) : Shape<DIM>(2) {
      this->point(0) = P0;
      this->point(1) = P1;
    }
    //@}
    /** @name Members
     */
    //@{
    inline Real measure() { return norm_2(this->point(1)
					  - this->point(0)); };
    //@}
  };
  
  template<int DIM>
  bool operator==(const Edge<DIM>& e1, const Edge<DIM>& e2) {
    return ( ( norm_2(e1.point(0) - e2.point(0)) == 0. &&
	       norm_2(e1.point(1) - e2.point(1)) == 0. ) ||
	     ( norm_2(e1.point(1) - e2.point(0)) == 0. &&
	       norm_2(e1.point(0) - e2.point(1)) == 0.) );
  }
  
  template<int DIM>
  Shape<DIM>::Shape(int nP) : M_nP(nP), M_nE(0), M_points(nP) {}
  
  template<int DIM>
  Shape<DIM>::Shape(const Shape& s) : M_nP(s.nP()), M_nE(s.nE()) {
    M_points.resize(M_nP);
    for(int i = 0; i < M_nP; i++)
      M_points[i] = s.point(i);
  }
  
  template<int DIM>
  std::ostream& operator<<(std::ostream& ostr, const Shape<DIM>& s) {
    ostr << "[" << s.nP() << "]" << std::endl;
    for(int i = 0; i < s.nP(); i++) {
      ostr << "P" << i << ": " << s.point(i);
      if(i < s.nP() -1)
	ostr << std::endl;
      else
	ostr << std::flush;
    }
    return ostr;
  }
}

#endif
