#include "fpdummy.hpp"

namespace FixedPoint {

  Dummy::Dummy( const Funp& f )
 :
    _f( f )
  {}

  double Dummy::apply( const double& x_k )
  {
    return x_k - _f( x_k );
  }
}
