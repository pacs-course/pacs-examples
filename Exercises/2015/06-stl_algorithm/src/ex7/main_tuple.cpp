// every ? should be replaced with a proper expression that exploits the use of tuples

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
  //? apply( ? );

private:
  param_T const M_param;
};

// ? apply( ? )
{
  func_T f, df;
  real x, x0;
  bool hasConverged = false;

  // use tie() to assign f,df,x0 from input
  // ?

  int it;
  for( int it = 0; it < get<1>( M_param ); it++ )
  {
    x = x0 - f( x0 ) / df ( x0 );
    x0 = x;

    cerr << "x    = " << x << std::endl;
    cerr << "f(x) = " << f( x ) << std::endl;

    // use get() to check tolerance from M_param
    //if( fabs( f( x ) ) < ? )
    {
       hasConverged = true;
       break;
    }
  }

  // use make_tuple to return
  // return ?;
}


int main()
{

  // use make_tuple to direclty create a Newton object
  // with tol = 1.e-10 e maxIt = 100
  //Newton n( ? );

  // build input tuple for apply()
  // using f, df and 2.
  // ? input( ? );

  // use auto to get the result from apply
  // ? result = n.apply( ? );

  bool hasConverged;
  real zero;

  // extract selectively from result the first and third
  // objects using tie() and ignore
  // ?

  cout << "the method has converged? " << boolalpha << hasConverged << std::endl;
  cout << "the zero is at   " << zero << std::endl;
  cout << "exact zero is at " << log(3.) - log(2.) << std::endl;

  return 0;
}
