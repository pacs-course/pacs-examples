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
  for (unsigned int i=0;i<ntimes;++i)
    {
      const double x=std::pow(10.5,5);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 
  std::cout<<"With optimized pow using type tags"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      const double x= pow<5>(10.5);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 

  std::cout<<"With the recursive constexpr version"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      // you need constexpr otherwise POW is
      // evaluated run time!
      constexpr double x=POW(10.5,5);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 
  //  std::cout<< Pow<11l,11>()<<std::endl;
  //std::cout<< pow<11>(11l)<<std::endl;
}
