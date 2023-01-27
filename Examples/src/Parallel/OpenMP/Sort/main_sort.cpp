/*
 * main_sort.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: forma
 */
#include <iostream>
#include <string>
#include "Utilities.hpp"//namespace vectorUtil
#include "GetPot"
#include "chrono.hpp"
void printHelp()
{
  std::cout<<"Sorts a vector using an odd-even transposition algorithm comparing with standard sort\n (and optionally the C++ parallel).\n\n";
  std::cout<<"main_sort -t num_threads -size number_of_elements\n";
  std::cout<<"num_threads: number of threads (default 2)\nnumber_of_elements: the size of the vector (a big number, default:1e5)\n";
}

template <class T>
bool testSorted(std::vector<T> const & a)
{
  bool res{true};
  for (std::size_t i=1u;i<a.size();++i)
    res = res && (a[i-1]<a[i]);
  return res;
}
int main(int argc, char **argv)
{
  Timings::Chrono clock;
  int n;
  std::vector<double> myVector;
  // get the data
  GetPot gp(argc,argv);
  if(gp.search(2, "-h", "--help"))
    {
      printHelp();
      return 0;
    }
  n = gp.follow(100000u, "-size");
  auto num_threads = gp.follow(2u, "-t");
  myVector.resize(n);
  // set the default number of threads
  omp_set_num_threads(num_threads);
  vectorUtil_OMP::fill_random(myVector); // fill the vector with random numbers
  std::vector<double> vectorCopy=myVector;
  std::cout<<"Sorting a vector of size "<<n<<" with "<<num_threads<<" threads\n";
  double time_std_sort;
  bool good_std_sort;
  {
    std::cout<<"Calling standard sort"<<std::endl;
    myVector=vectorCopy;
    clock.start();
    vectorUtil_OMP::sort_vec(myVector);
    clock.stop();
    time_std_sort=clock.wallTime();
    good_std_sort=testSorted(myVector);
    std::cout<<"Done!"<<std::endl;
  }
/*
  double time_oddeven;
  bool good_oddeven;

  {
    std::cout<<"Calling OMP parallel odd even sort"<<std::endl;
    myVector=vectorCopy;
    clock.start();
    vectorUtil_OMP::Odd_even(myVector);
    clock.stop();
    time_oddeven=clock.wallTime();
    good_oddeven=testSorted(myVector);
    std::cout<<"Done!"<<std::endl;
 }
*/
  double time_quickpar;
  bool good_quickpar;
  {
    std::cout<<"Calling OMP parallel quicksort"<<std::endl;
    clock.start();
    myVector=vectorCopy;
    vectorUtil_OMP::parallelQuickSort(myVector);
    clock.stop();
    time_quickpar=clock.wallTime();
    good_quickpar=testSorted(myVector);
    std::cout<<"Done!"<<std::endl;
    good_quickpar=testSorted(myVector);
 }
#ifdef CPP_PARALLEL
  double time_std_sort_par;
  bool good_std_sort_par;

  {
    std::cout<<"Calling standard parallel sort"<<std::endl;
    myVector=vectorCopy;
    clock.start();
    vectorUtil_OMP::sort_vec_par(myVector);
    clock.stop();
    time_std_sort_par=clock.wallTime();
    good_std_sort_par=testSorted(myVector);
    std::cout<<"Done!"<<std::endl;
 }
#endif
  std::cout<<"Time for C++ standard sort: "<<time_std_sort<<" msec"<<" Well sorted?"<<std::boolalpha<<good_std_sort<<std::endl;
//  std::cout<<"Time for OMP odd-even sort: "<<time_oddeven <<<<" msec"<<" Well sorted?"<<std::boolalpha<<good_oddeven<<std::endl;
  std::cout<<"Time for OMP quick-sort:    "  <<time_quickpar <<" msec"<<" Well sorted?"<<std::boolalpha<<good_quickpar<<std::endl;
#ifdef CPP_PARALLEL
  std::cout<<"Time for C++ parallel sort: "<<time_std_sort_par<<" msec"<<" Well sorted?"<<std::boolalpha<<good_std_sort_par<<std::endl;
#endif

  return 0;

}




