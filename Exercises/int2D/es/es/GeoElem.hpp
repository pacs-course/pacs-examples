#ifndef _GEOELE_H 
#define _GEOELE_H 1
#include <vector>

namespace Geometry
{

  const int ndim=2;

  class Point
  {
  
  public:
  
    explicit  Point( const double& x = 0, 
		     const double& y = 0);

    Point(const Point &);
    Point & operator=(const Point&);
    
    // We can get the coordinates also by operator []
    double const operator[](int i)const {return M_coor[i];}
    double & operator[](int i) {return M_coor[i];}
    
    friend Point operator +(const Point&, const Point &);
    friend Point operator -(const Point&, const Point &);
    Point operator *(const double &)const;
    friend Point operator*(const double &, const Point &);
    
  private:

    std::vector<double> M_coor;
  };

  class Triangle
  {

  public:

    static const int numVertices=3;
    Triangle(); //Constructs an empty triangle
    Triangle( const Point&,
	      const Point&,
	      const Point& ); //Points are given (by reference)
    Triangle( const Triangle& );

    Triangle & operator=( const Triangle& );

    inline Point const & operator[]( int i ) const 
    {
      return M_points[i];
    }
    // Can be used ONLY if empty()==false
    inline Point & operator[]( int i )
    {
      return M_points[i];
    }

  private:

    std::vector<Point>  M_points;

  };

}

#endif
