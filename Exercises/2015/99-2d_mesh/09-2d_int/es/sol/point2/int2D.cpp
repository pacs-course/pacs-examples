#include "int2D.hpp"
#include "fekete.H"
namespace NumericalIntegration
{
  
  Quadrature::Quadrature( const Geometry::Mesh2D & mesh ):
    M_mesh(mesh)
  {}

  void Quadrature::triangleFekete( const Geometry::Triangle& T, double* Tf )
  {
    for( int i(0); i<3; ++i)
      {
	Tf[2*i] = T[i][0];
	Tf[2*i+1] = T[i][1];
      }
    
  }
  
  double Quadrature::applyTriangle( FunPoint f, const Geometry::Triangle& T, int rule )
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

  double Quadrature::apply( FunPoint f, int rule )
  {
    double I(0);
    for ( unsigned int i = 0 ; i < M_mesh.numTriangles(); ++i) 
      {
	I += applyTriangle( f, M_mesh[i], rule );
      }
    return I;
  }
}
