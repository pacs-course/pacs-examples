#include "serialize.h"
#include <iostream>
#include <map>

int
main ()
{
  std::vector<double> a(10, 79.1234);
  
  std::vector<unsigned char> sa = write (a);
  
  std::cout << "sa = " << std::endl;
  for (int ii = 0; ii < sa.size (); ++ii)
    std::cout << sa[ii];
  std::cout << std::endl;

  std::vector<double> b = read<double> (sa);
  std::cout << "b = " << std::endl;
  for (int ii = 0; ii < b.size (); ++ii)
    std::cout << b[ii] <<  std::endl;

  std::map<int, double> c;
  c[0] = 1.;
  c[12] = 3.14;
  std::vector<unsigned char> sc = write_map (c);

  std::cout << "sc = " << std::endl;
  for (int ii = 0; ii < sc.size (); ++ii)
    std::cout << sc[ii];
  std::cout << std::endl;

  std::map<int, double> d = read_map<int, double> (sc);
  std::cout << "d = " << std::endl;
  for (auto ii = d.begin (); ii != d.end (); ++ii)
      std::cout << ii->first << " " << ii->second << std::endl;
  std::cout << std::endl;
  
  return 0;
}
