#include "functions.hpp"
#include "functions2.hpp"
double fun2(double x){return x*x;}
int main()
{
  fun(5.0);
  fun(3);
  double b(9);
  fun(b);
  int i(0);
  fun(i);
  long int z(0l);
  fun(z);
  fun(&z);
  float ff(0.);
  fun(ff);
  funct(b);
  funct(fun2(3.0));
   
}
