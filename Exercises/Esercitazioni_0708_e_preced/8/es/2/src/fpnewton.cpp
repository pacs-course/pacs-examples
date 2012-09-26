#include "fpnewton.hpp"

namespace FixedPoint {

  Newton::Newton( const Funp& f, const Funp& df )
 :
    _f( f ),
    _df( df )
  {}

  double Newton::apply( const double& x_k )
  {
    return x_k - ( _f( x_k ) / _df( x_k ) );
  }
}
