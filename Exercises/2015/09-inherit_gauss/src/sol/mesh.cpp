#include "mesh.hpp"

namespace Generator
{
  // Uniform function generator.
  std::vector<double> uniform ( double const & a, double const & b, unsigned int const & n )
  {
    // Create the empty vector of nodes.
    std::vector<double> mesh ( n + 1, 0. );

    // Compute the uniform size of the mesh.
    double h = ( b - a ) / static_cast<double>(n);

    // Loop to fill the nodes.
    for( unsigned int i=0; i<n + 1; ++i )
    {
      mesh[i] = a + h * static_cast<double>(i);
    }

    // Return the mesh nodes.
    return mesh;
  } // uniform

} // namespace Generator

namespace Geometry
{

  // Constructor.
  Mesh1D::Mesh1D ( const Domain1D& d,
                   const GetPot& data,
                   const std::string& section,
                   Splitter split ):
    M_domain ( d ),
    M_nodes ( split( M_domain.left(),
    M_domain.right(),
    data( ( section + "nint" ).data(), 10 ) ) )
  {} // Constructor.

  // Virtual destructor.
  Mesh1D::~Mesh1D ()
  {} // Virtual destructor.

  // Size of the mesh
  double Mesh1D::h() const
  {

    // Create a temporary vector.
    std::vector<double> tmp ( M_nodes.size() );

    // Put in the temporary vector the difference between all points.
    std::adjacent_difference ( tmp.begin(), tmp.end(), tmp.begin() );

    // Compute the maximum value of the differences and return the value.
    return *std::max_element ( tmp.begin() + 1, tmp.end() );

  } // h

} // namespace Geometry
