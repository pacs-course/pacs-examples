#include "Pow.hpp"
#include <iostream>
#include <cmath>
#include "chrono.hpp"
int main()
{
  Timings::Chrono watch;
  const unsigned int ntimes=1000;
  // I compute 10.5^5
  std::cout<<"With standard pow"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)std::pow(10.5,5);
  watch.stop();
  std::cout<<watch<<std::endl; 
  std::cout<<"With optimized pow (template)"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)pow<5>(10.5);
  watch.stop();
  std::cout<<watch<<std::endl; 

  std::cout<<"With the recursive version"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)POW(10.5,5);
  watch.stop();
  std::cout<<watch<<std::endl; 

  //  std::cout<< Pow<11l,11>()<<std::endl;
  //std::cout<< pow<11>(11l)<<std::endl;
}
