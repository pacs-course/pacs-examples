#include "serialize.h"
#include <iostream>
#include <mpi.h>

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

  // write and read back an std::map<int, double>  
  std::map<int, double> c; c[0] = 1.; c[12] = 3.14;
  std::vector<unsigned char> sc = serialize::write (c);
  std::map<int, double> d;
  serialize::read (sc, d);
  
  std::cout << "sc = " << std::endl;
  for (int ii = 0; ii < sc.size (); ++ii)
    std::cout << sc[ii];
  std::cout << std::endl;

  std::cout << "c = " << std::endl;
  for (auto ii = c.begin (); ii != c.end (); ++ii)
      std::cout << ii->first << " " << ii->second << std::endl;
  std::cout << std::endl;


  std::cout << "d = " << std::endl;
  for (auto ii = d.begin (); ii != d.end (); ++ii)
      std::cout << ii->first << " " << ii->second << std::endl;
  std::cout << std::endl;

  
  // write and read back an std::vector<std::map<int, double>>
  std::vector<std::map<int, double>> vm (5); vm[3][2] = 3.14; vm[4][8] = 31.4;
  std::vector<unsigned char> svm = serialize::write (vm);
  std::vector<std::map<int, double>> e;
  serialize::read (svm, e);
  
  std::cout << "svm = " << std::endl;
  for (int ii = 0; ii < svm.size (); ++ii)
    std::cout << svm[ii];
  std::cout << std::endl;

  std::cout << "vm = " << std::endl;
  for (int ii = 0; ii < vm.size (); ++ii)
    {
      std::cout << "map #" << ii << std::endl;
      for (auto jj = vm[ii].begin (); jj != vm[ii].end (); ++jj)
        std::cout << jj->first << " " << jj->second << std::endl;
    }
  std::cout  << std::endl;

  std::cout << "e = " << std::endl;
  for (int ii = 0; ii < e.size (); ++ii)
    {
      std::cout << "map #" << ii << std::endl;
      for (auto jj = e[ii].begin (); jj != e[ii].end (); ++jj)
        std::cout << jj->first << " " << jj->second << std::endl;
    }

  return 0;
}
