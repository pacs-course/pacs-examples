#include "serialize.h"
#include <iostream>

int
main (int argc, char **argv)
{  
  // write and read back an std::vector<double>
  std::vector<double> a(10, 79.1234);  
  std::vector<unsigned char> sa = serialize::write (a);
  std::vector<double> b;
  serialize::read (sa, b);
  
  std::cout << "sa = " << std::endl;
  for (int ii = 0; ii < sa.size (); ++ii)
    std::cout << sa[ii];
  std::cout << std::endl;

  
  std::cout << "b = " << std::endl;
  for (int ii = 0; ii < b.size (); ++ii)
    std::cout << b[ii] <<  std::endl;


  return 0;
}
