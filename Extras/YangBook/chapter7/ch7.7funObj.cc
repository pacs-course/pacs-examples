#include <cmath>
#include <iostream>

template<class Fcn>
double trapezoidal(double a, double b, Fcn f, int n) {
  double h = (b - a)/n;       // size of each subinterval
  double sum = f(a)*0.5;
  for (int i = 1; i < n; i++) sum += f(a + i*h);
  sum += f(b)*0.5;
  return sum*h;
}

template<class T, class Fcn>
T trapezoidal2(T a, T b, Fcn f, int n) {
  T h = (b - a)/n;       // size of each subinterval
  T sum = f(a)*0.5;
  for (int i = 1; i < n; i++) sum += f(a + i*h);
  sum += f(b)*0.5;
  return sum*h;
}

template<double F(double)>
double trapezoidal3(double a, double b, int n) {
  double h = (b - a)/n;       // size of each subinterval
  double sum = F(a)*0.5;
  for (int i = 1; i < n; i++) sum += F(a + i*h);
  sum += F(b)*0.5;
  return sum*h;
}

template<class T, T F(T)>
T trapezoidal4(T a, T b, int n) {
  T h = (b - a)/n;       // size of each subinterval
  T sum = F(a)*0.5;
  for (int i = 1; i < n; i++) sum += F(a + i*h);
  sum += F(b)*0.5;
  return sum*h;
}

double myintegrand(double x) {
  return exp(-x*x);
}

class integrand {      // define a class for my integrand
public:
  double operator()(double x) { return std::exp(-x*x); }
};

void f() {
  std::cout << trapezoidal(0, 1, integrand(), 100) << '\n';
  std::cout << trapezoidal3<myintegrand>(0, 1, 100) << '\n';
  std::cout << trapezoidal4<double, myintegrand>(0, 1, 100) << '\n';
}

template<class T> class integrand2 {
public:                  // define a class for my integrand
  T operator()(T x) { return std::exp(-x*x); }
};

int main() {
  f();
  std::cout << trapezoidal2(0.0, 1.0, integrand2<double>(), 100)
       << '\n';
}

