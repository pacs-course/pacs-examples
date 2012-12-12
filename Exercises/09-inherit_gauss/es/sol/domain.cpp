#include "domain.hpp"

namespace Geometry
{

  // Constructor.
  Domain1D::Domain1D ( const GetPot& data,
                       const std::string & section ):
    M_a ( data( ( section + "a").data(), 0. ) ),
    M_b ( data( ( section + "b").data(), 2. ) )
  {} // Constructor.

  // Virtual destructor.
  Domain1D::~Domain1D ()
  {} // Virtual destructor.

} // namespace Geometry
