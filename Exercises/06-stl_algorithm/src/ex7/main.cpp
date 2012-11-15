#include <tuple>
#include <iostream>
#include <cmath>

using namespace std;

typedef double real;

real f( real x )
{ return exp(x) - 1.5; }

real df( real x )
{ return exp(x); }

class Newton
{
public:
  // real function
  typedef real(*func_T)( real const );

  // input for the constructor
  // tolerance, max iterations
  typedef tuple<real,int> param_T;

  // input for the apply method
  // function, derivative, starting point
  typedef tuple<func_T,func_T,real> applyInput_T;

  //return type of the apply method
  // bool -> say if the method converged
  // int  -> number of iterations
  // real -> zero
  typedef tuple<bool,int,real> applyOutput_T;

  Newton( param_T const & param ): M_param( param ) {}

  // define the apply method using tuples
  applyOutput_T apply( applyInput_T const & input );

private:
  param_T const M_param;
};

Newton::applyOutput_T Newton::apply( applyInput_T const & input )
{
  func_T f, df;
  real x, x0;
  bool hasConverged = false;

  // use tie()
  tie( f, df, x0 ) = input;

  int it;
  for( int it = 0; it < get<1>( M_param ); it++ )
  {
    x = x0 - f( x0 ) / df ( x0 );
    x0 = x;

    cerr << "x    = " << x << std::endl;
    cerr << "f(x) = " << f( x ) << std::endl;
    // get tolerance
    if( fabs( f( x ) ) < get<0>( M_param ) )
    {
       hasConverged = true;
       break;
    }
  }

  // use make_tuple
  return make_tuple( hasConverged, it, x );
}


int main()
{

  // use make_tuple
  Newton n( make_tuple( 1.e-8, 100 ) );

  // build input tuple
  tuple<Newton::func_T,
        Newton::func_T,
        real> input( f, df, 2.0 );

  // use auto
  auto result = n.apply( input );

  bool hasConverged;
  real zero;

  // extract selectively first and third
  tie( hasConverged, ignore, zero ) = result;

  cout << "the method has converged? " << boolalpha << hasConverged << std::endl;
  cout << "the zero is at   " << zero << std::endl;
  cout << "exact zero is at " << log(3.) - log(2.) << std::endl;

  return 0;
}
