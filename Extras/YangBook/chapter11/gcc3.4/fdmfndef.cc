// fndef.cc

#include "fdm.h"

template<class T, class R> R fdm<T,R>::a11(T x, T y){
  return exp(x);
}

template<class T, class R> R fdm<T,R>::a12(T x, T y){
  return x;
}

template<class T, class R> R fdm<T,R>::a22(T x, T y){
  return exp(y);
}

template<class T, class R> R fdm<T,R>::b1(T x, T y){
  return 100.0+x+y;
}

template<class T, class R> R fdm<T,R>::b2(T x, T y){
  return 1.0+3*x -50*y;
}

template<class T, class R> R fdm<T,R>::rcc(T x, T y){
  return x*(1 + y*(49 + x));
}

template<class T, class R> R fdm<T,R>::bdg(T x, T y){
  return exp(x*y);
}


template<class T, class R> R fdm<T,R>::scf(T x, T y){
  return - (1 + y)*y*exp(x*(1+y)) - (1 + x)*x*exp(y*(1+x))
         + (3*x*x + y*(100 + y))*exp(x*y);
}
