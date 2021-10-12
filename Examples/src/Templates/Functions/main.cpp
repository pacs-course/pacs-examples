#include "functions.hpp"
#include "functions2.hpp"
double
fun2(double x)
{
  return x * x;
}
int
main()
{
  fun(5.0);
  fun(3);
  double b(9);
  fun(b);
  int i(0);
  fun(i);
  const long int z(0l);
  fun(z);
  fun(&z);
  float ff(0.);
  fun(ff);
  std::cout << " Now calling funct\n";
  funct(b);
  funct(fun2(3.0));
  funct(3.5);
  const long double c = 9.0l;
  funct(c);
}
