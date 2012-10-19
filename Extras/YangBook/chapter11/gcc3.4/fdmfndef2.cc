// file fndef2.cc

#include "fdm.h"

template<class T, class R> R fdm<T,R>::a11(T x, T y){
  return 1;
}

template<class T, class R> R fdm<T,R>::a12(T x, T y){
  return 0;
}

template<class T, class R> R fdm<T,R>::a22(T x, T y){
  return 1;
}

template<class T, class R> R fdm<T,R>::b1(T x, T y){
  return 0;
}

template<class T, class R> R fdm<T,R>::b2(T x, T y){
  return 0;
}

template<class T, class R> R fdm<T,R>::rcc(T x, T y){
  return (-exp(-x*y), 1);
}

template<class T, class R> R fdm<T,R>::bdg(T x, T y){
  return exp((0, x+y));
}

template<class T, class R> R fdm<T,R>::scf(T x, T y){
  return (2 - exp(-x*y) + (0,1))*exp((0,x+y));
}
