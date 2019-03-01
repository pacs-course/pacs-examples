/* epoch.c: A program to show the time since the Epoch */
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "../config.h"
#include "epoch.h"



int main(int argc, char* argv[])
{
  printf("%f\n", get_epoch());
  return 0;
}

double get_epoch ()
{
  double sec;
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tv;
  gettimeofday(&tv, NULL);
  sec = tv.tv_sec;
  sec += tv.tv_usec / 1000000.0;
#else
  sec = time(NULL);
#endif
  return sec;
}
