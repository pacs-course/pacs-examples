#include <functional>
#include <cmath>

class
var
{

  std::function<double (double)> val;
  std::function<double (double)> der; 

public:

  var ()
    : val ([] (double x) -> double { return x; }),
      der ([] (double x) -> double { return 1.0; })
  { };

  var (std::function<double (double)> val_,
       std::function<double (double)> der_)
    : val (val_), der (der_)
  { };

  double
  eval (double x) const
  { return val (x); };

  double
  eval_der (double x) const
  { return der (x); };  
};

var sin (var X);
var cos (var X);

var operator* (double a, var X);
var operator* (var X, double a);
var operator* (var Y, var X);

var operator+ (double a, var X);
var operator+ (var X, double a);
var operator+ (var Y, var X);

var operator^ (var X, int n);
