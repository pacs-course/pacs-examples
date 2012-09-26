#include <iostream>
#include "fekete.H"
#include "GeoElem.hpp"

using namespace Geometry;

typedef double (*FunPoint) ( const double&, const double & );

double f1 ( const double&, const double& );
double f2 ( const double&, const double& );

void triangleFekete( const Triangle&, double* );

double int2D( const FunPoint&, const Triangle&, int );

int main( int argc, char* argv[] )
{

  // Create the reference triangle
  Triangle hatT( Point(0,0), 
		 Point(1,0),
		 Point(0,1) );
  
  std::cout << "Integral value f1 " << int2D( f1, hatT, 5 ) << std::endl;

  Triangle T( Point(2, 2),
	      Point(3, 5),
	      Point(1, 3) );

  
  std::cout << "Integral value f2 " << int2D( f2, T, 5 ) << std::endl;

  return 0;
}

double f1( const double& x, const double& y )
{
  return 1;
}

double f2( const double& x, const double& y )
{
  return x;
}

double int2D( const FunPoint& f, const Triangle& T, int rule )
{

  double I(0), x, y;
  double *hatXY, *XY, *w;

  // Transform the triangle to the Fekete style triangle
  double Tf[6];
  triangleFekete( T, Tf );

  // Returns the order of a given Fekete rule for the triangle.
  int order_num = fekete_order_num( rule );

  // Vector of nodes, in reference triangle and acutual triangle, and weights.
  hatXY = new double[2 * order_num];
  XY = new double[2 * order_num];
  w = new double[order_num];
	
  // Nodes and weights in reference triangle
  fekete_rule( rule, order_num, hatXY, w);
	
  // Map nodes to the acutal triangle
  reference_to_physical_t3(Tf, order_num, hatXY, XY);

  // Compute the integral
  for ( int order = 0; order < order_num; ++order )
    {
      x = XY[0 + order * 2];
      y = XY[1 + order * 2];	    
     
      I += w[order] * f(x, y);
    }

  I *= triangle_area( Tf );

  delete [] hatXY;
  delete [] XY;
  delete [] w;

  return I;
}

void triangleFekete( const Triangle& T, double* Tf )
{
  for( int i(0); i<3; ++i)
    {
      Tf[2*i] = T[i][0];
      Tf[2*i+1] = T[i][1];
    }

}

