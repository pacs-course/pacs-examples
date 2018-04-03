#include "ad.h"
#include <iostream>

var
sin (var X)
{
  //  std::cout << "sin (var X)" << std::endl;
  var y([X] (double x) -> double
        { return sin (X.eval (x)); },
        [X] (double x) -> double
        { return cos (X.eval (x)) * X.eval_der (x); });
  return y;
};


var
cos (var X)
{
  //  std::cout << "cos (var X)" << std::endl;
  var y([X] (double x) -> double
        { return cos (X.eval (x)); },
        [X] (double x) -> double
          { return - sin (X.eval (x)) * X.eval_der (x); });
  return y;
};

var
exp (var X)
{
  //  std::cout << "exp (var X)" << std::endl;
  var y([X] (double x) -> double
        { return exp (X.eval (x)); },
        [X] (double x) -> double
        { return exp (X.eval (x)) * X.eval_der (x); });
  return y;
};


var
operator* (double a, var X)
{
  //  std::cout << "operator* (double a, var X)" << std::endl;
  var y([a, X] (double x) -> double
        { return a * X.eval (x); },
        [a, X] (double x) -> double
        { return a * X.eval_der (x); });
  return y;
}

var
operator* (var X, double a)
{
  //  std::cout << "operator* (var Y, var X)" << std::endl;
  return (a * X);
}

var
operator* (var Y, var X)
{
  //  std::cout << "operator* (var Y, var X)" << std::endl;
  var z ([Y,X] (double x) -> double
         { return Y.eval (x) * X.eval (x); },
         [Y,X] (double x) -> double
         { return Y.eval_der (x) * X.eval (x) + Y.eval (x) * X.eval_der (x); });
  return z;
}

var
operator+ (double a, var X)
{
  //  std::cout << "operator+ (double a, var X)" << std::endl;
  var y([a, X] (double x) -> double
        { return a + X.eval (x); },
        [a, X] (double x) -> double
        { return X.eval_der (x); });
  return y;
}

var
operator+ (var X, double a)
{
  //  std::cout << "operator+ (var X, double a)" << std::endl;
  return (a + X);
}

var
operator+ (var Y, var X)
{
  //  std::cout << "operator+ (var Y, var X)" << std::endl;
  var z([Y,X] (double x) -> double
        { return Y.eval (x) + X.eval (x); },
        [Y,X] (double x) -> double
        { return Y.eval_der (x) + X.eval_der (x); });
  return z;
}


var
operator^ (var X, int n)
{
  //  std::cout << "operator^ (var X, int n)" << std::endl;
  std::function<double (double)> val, der;
  
  if (n == 0)
    {
      val = [] (double x) -> double
        { return 1.0; };
      der = [] (double x) -> double
        { return 0.0; };
    }
  else
    {
      val = [X,n] (double x) -> double
        { return std::pow (X.eval (x), n); };
      der = [X,n] (double x) -> double
        { return  n * std::pow (X.eval (x), n-1) * X.eval_der (x); };
    }
  var z = var (val, der);
  return (z);
};

var
operator- (double a, var X)
{ return a + ((-1.0) * X); };

var
operator- (var X, double a)
{ return X + (-a); };

var
operator- (var Y, var X)
{ return Y + ((-1.0) * X); };

var
operator/ (double a, var X)
{ return a * (X^(-1)); };

var operator/ (var X, double a)
{ return (1/a) * X; };

var operator/ (var Y, var X)
{ return Y * (X^(-1)); };
