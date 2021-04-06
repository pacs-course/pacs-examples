#ifndef H_FUNCTION_HPP
#define H_FUNCTION_HPP
// I need extern "C" to avoid name mangling
extern "C"{
  double square(double x);
  double cube(double x);
  double strangefun(double x);
}
#endif
