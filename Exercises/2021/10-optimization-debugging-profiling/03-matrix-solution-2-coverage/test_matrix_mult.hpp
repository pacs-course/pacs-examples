#ifndef HAVE_TEST_MATRIX_MULT_H
#define HAVE_TEST_MATRIX_MULT_H

#ifndef msize
#  define msize 100
#endif

#include <ctime>

static clock_t c_start, c_diff;
static double  c_sec;
#define tic() c_start = clock();
#define toc(x)                                      \
  c_diff = clock() - c_start;                       \
  c_sec  = (double)c_diff / (double)CLOCKS_PER_SEC; \
  std::cout << x << c_sec << " [s]" << std::endl;

#endif
