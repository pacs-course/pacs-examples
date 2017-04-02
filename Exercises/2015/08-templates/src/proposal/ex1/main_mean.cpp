#include <iostream>
#include <cmath>
#include <cassert>

typedef float real;

struct ArithmeticMean
{
  static real sum( real const & x1, real const & x2 ) { return 0.5 * ( x1+x2 ); }
};

struct HarmonicMean
{
  static real sum( real const & x1, real const & x2 ) { return ( x1*x2 ) / (x1+x2); }
};

struct GeometricMean
{
  static real sum( real const & x1, real const & x2 ) { return std::sqrt( x1*x2 ); }
};

template <typename meanT=ArithmeticMean>
class Point2D
{
public:
  Point2D( std::initializer_list<real> il )
  {
    assert( il.size() == 2 );
    int i = 0;
    for ( auto v: il )
      M_data[i++] = v;
  }
  real const & x() const { return M_data[0]; }
  real const & y() const { return M_data[1]; }
private:
  float M_data[2];
};

template <typename meanT=ArithmeticMean>
Point2D<meanT> operator+( const Point2D<meanT> & p1, const Point2D<meanT> & p2 )
{
  return { meanT::sum( p1.x(), p2.x() ), meanT::sum( p1.y(), p2.y() ) };
}

int main()
{
  Point2D<GeometricMean> p1{1,3},p2{2,5},p3{0,1};

  std::cout << p1.x() << " " << p1.y() << std::endl;
  std::cout << p2.x() << " " << p2.y() << std::endl;
  
  p3 = p1 + p2;

  std::cout << p3.x() << " " << p3.y() << std::endl;

  return 0;
}

