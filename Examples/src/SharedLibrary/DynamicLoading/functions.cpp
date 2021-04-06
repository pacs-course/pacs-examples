#include<cmath>
extern "C"{
  double square(double x){return x*x;}
  double cube(double x){return x*x*x;}
  double strangefun(double x){
    return std::cosh(x)+std::exp(-x)*erf(x);
  }

}
