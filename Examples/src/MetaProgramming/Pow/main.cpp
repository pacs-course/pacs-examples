#include "Pow.hpp"
#include <iostream>
#include <cmath>
#include <complex>
#include "chrono.hpp"
int main()
{
  Timings::Chrono watch;
  const unsigned int ntimes=1000;
  // I compute 10.5^8
  std::cout<<"With standard pow"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      const double x=std::pow(10.5,8);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 
  std::cout<<"With optimized pow using type tags"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      const double x= pow<8>(10.5);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 

  std::cout<<"With the recursive constexpr version"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      double x=POW(10.5,8);
    }
  watch.stop();
  std::cout<<watch<<std::endl; 

  std::cout<<"With the c++17 version"<<std::endl;
  watch.start();
  for (unsigned int i=0;i<ntimes;++i)
    {
      double x=pow17<8>(10.5);
    }
   watch.stop();
   std::cout<<watch<<std::endl;
//! But it works also with complex numbers (all of them can!)

   std::cout<<"With the c++17 version with a complex number"<<std::endl;
    watch.start();
    for (unsigned int i=0;i<ntimes;++i)
      {
        std::complex<double> x=pow17<8>(std::complex<double>{10.1,3.0});
      }
     watch.stop();
     std::cout<<watch<<std::endl;




  //  std::cout<< Pow<11l,11>()<<std::endl;
  //std::cout<< pow<11>(11l)<<std::endl;
}
