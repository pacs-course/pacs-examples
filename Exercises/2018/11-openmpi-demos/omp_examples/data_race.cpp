#include <omp.h>
#include <iostream>

#define N 1024
int main (void)
{
  
  int a[N], b[N], i, nt;
  for (i = 0; i < N; i++)
    {a[i] = i; b[i] = 2*1;}
  


  //serial version
  int serial_checksum = 0;
  for (i=0; i < N-1; i++)
    {
      a[i] = a[i+1] + b[i];   
      serial_checksum += a[i];   
    }
  


  for (i = 0; i < N; i++)
    {a[i] = i; b[i] = 2*1;}

  //parallel version
  int parallel_checksum = 0;
#pragma omp parallel shared (a, b, nt)
  {
#pragma omp master
    {
      nt =  omp_get_num_threads ();
      std::cout << "number of threads: " << nt;
    }
#pragma omp for
    for (i=0; i < N-1; i++)
      {
	a[i] = a[i+1] + b[i];   
      }

  }

  for (i=0; i < N-1; i++)
    {
      parallel_checksum += a[i];
    }
  
  std::cout << "\tcorrect result: " << serial_checksum << 
    "\tparallel result: " << parallel_checksum << std::endl;

}
