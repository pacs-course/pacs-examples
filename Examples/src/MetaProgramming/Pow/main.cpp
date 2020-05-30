#include "Pow.hpp"
#include "chrono.hpp"
#include "generalPow.hpp"
#include <cmath>
#include <complex>
#include <iostream>
int
main ()
{
  using namespace apsc;
  Timings::Chrono    watch;
  const unsigned int ntimes = 10000000;
  double x=0;
  std::complex<double> y{0.,0.};
  // I compute 10.5^4
  std::cout << "With standard pow" << std::endl;
  watch.start ();
  for (unsigned int i = 0; i < ntimes; ++i)
    {
      x += std::pow (10.5, 4.0);
    }
  watch.stop ();
  std::cout<<x<<std::endl;
  std::cout << watch << std::endl;
  std::cout << "With optimized pow using type tags" << std::endl;
  x=0;
  watch.start ();
  for (unsigned int i = 0; i < ntimes; ++i)
    {
      x += pow<4> (10.5);
    }
  watch.stop ();
  std::cout<<x<<std::endl;
  std::cout << watch << std::endl;

  std::cout << "With the recursive constexpr version" << std::endl;
  x=0;
  watch.start ();
  for (unsigned int i = 0; i < ntimes; ++i)
    {
      x += POW (10.5, 4);
    }
  watch.stop ();
  std::cout<<x<<std::endl;
  std::cout << watch << std::endl;

  std::cout << "With the c++17 version" << std::endl;
  x=0;
  watch.start ();
  for (unsigned int i = 0; i < ntimes; ++i)
    {
      x += pow17<4> (10.5);
    }
  watch.stop ();
  std::cout<<x<<std::endl;
  std::cout << watch << std::endl;
  //! But it works also with complex numbers (all of them can!)

  std::cout << "With the c++17 version with a complex number" << std::endl;
  watch.start ();
  for (unsigned int i = 0; i < ntimes; ++i)
    {
      y += pow17<4> (std::complex<double>{10.1, 4.0});
    }
  std::cout<<y<<std::endl;
  watch.stop ();
  std::cout << watch << std::endl;

  std::cout<<"Now the general pow\n";
  std::cout << "With unsigned int exponent" << std::endl;
    watch.start ();
    x=0;
    for (unsigned int i = 0; i < ntimes; ++i)
      {
        x += apsc::pow(10.5,4u);
      }
    watch.stop ();
    std::cout<<x<<std::endl;
    std::cout << watch << std::endl;
    std::cout << "With signed and negative int exponent" << std::endl;
    x=0;
    watch.start ();
    for (unsigned int i = 0; i < ntimes; ++i)
      {
        x += apsc::pow(10.5,-4);
      }
    watch.stop ();
    std::cout<<x<<std::endl;
    std::cout << watch << std::endl;
    std::cout << "With float exponent" << std::endl;
    x=0;
    watch.start ();
    for (unsigned int i = 0; i < ntimes; ++i)
      {
        x += apsc::pow(10.5,4.0);
      }
    std::cout<<x<<std::endl;
    watch.stop ();
    std::cout << watch << std::endl;


  //  std::cout<< Pow<11l,11>()<<std::endl;
  // std::cout<< pow<11>(11l)<<std::endl;
}
