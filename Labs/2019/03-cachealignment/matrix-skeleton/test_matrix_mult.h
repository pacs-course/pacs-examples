#ifndef HAVE_TEST_MATRIX_MULT_H
#define HAVE_TEST_MATRIX_MULT_H

#ifndef msize
#define msize 100
#endif

static clock_t c_start, c_diff;
static double c_msec;
#define tic() c_start = clock ();
#define toc(x) c_diff = clock () - c_start;                   \
  c_msec = (double)c_diff * 1000 / (double)CLOCKS_PER_SEC;    \
  std::cout << x << c_msec << std::endl;

#endif
