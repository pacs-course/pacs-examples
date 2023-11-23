
/*
 * main_fibs.hpp
 *
 *  Created on: Aug 26, 2022
 *      Author: forma
 */
#include <iostream>
#include "chrono.hpp"
#include "GetPot"
#include "Fibonacci.hpp"
void
printHelp()
{
  std::cout << "Computes Fibonacci sequences. To run the program\n";
  std::cout << "./main_fibs -t num_threads -l task_limit\n";
  std::cout
    << "num_threads: number of threads requested (def. 2),\n"
    << "task_limit when the parallel code computes fib(n)\n<<"<<
    " with n<task_limit it resorts to the serial version (def 1)\n";
    std::cout<< std::endl;
}
int
main(int argc, char **argv)
{
  GetPot gp(argc, argv);
  if(gp.search(2, "-h", "--help"))
    {
      printHelp();
      return 0;
    }
  Fibonacci::num_threads=gp.follow(2u, "-t");
  Fibonacci::par_limit=gp.follow(1u, "-l");
  std::cout<<"Using "<<Fibonacci::num_threads<<" threads"<<" and parallel limit "<<Fibonacci::par_limit<<std::endl;
  unsigned int n;
  bool stop(false);
  while(!stop)
    {
      std::cout<<"Gimme the number of fibonacci number we want to compute (0 to stop)"<<std::endl;
      std::cin>>n;
      std::cout<<"Scalar version\n";
      Fibonacci::Fibonacci_s f_s;
      Timings::Chrono clock;
      clock.start();
      auto res_s=f_s.compute(n);
      clock.stop();
      auto scalar_time=clock.wallTime();
      std::cout<<"done\n";

       std::cout<<"Scalar non recursive version\n";
       Fibonacci::Fibonacci_nonrec f_nonrec;
       clock.start();
       auto res_nonrec=f_nonrec.compute(n);
       clock.stop();
       auto nonrec_time=clock.wallTime();
       std::cout<<"done\n";


      std::cout<<"Parallel version\n";
      Fibonacci::Fibonacci f;
      clock.start();
      auto res=f.compute(n);
      clock.stop();
      auto parallel_time=clock.wallTime();
      std::cout<<"done\n";

      std::cout<<"Results:\n";
      std::cout<<"Serial    version, value="<<res_s[n]<<" Time:"<<scalar_time<<"microsec\n";
      std::cout<<"Serial nr version, value="<<res_nonrec[n]<<" Time:"<<nonrec_time<<"microsec\n";
      std::cout<<"Parallel  version, value="<<res[n]<<" Time:"<<parallel_time<<"microsec\n";
    }

}


