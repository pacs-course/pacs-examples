// from cppreference.com

// With tuple_cat you can concateneta tuples but not only tuples!

#include "printtuple.hpp"
#include <iostream>
#include <string>
#include <tuple>
int
main()
{
  std::tuple<int, std::string, float> t1(10, "Test", 3.14);
  std::cout << "t1= " << t1 << std::endl;
  int n = 7;
  // u=you can concatenete tuples with tuples, pairs or individual variables
  // usinf std::tie!!

  auto t2 = std::tuple_cat(t1, std::make_pair("Foo", "bar"), t1, std::tie(n));
  // Now t2 is a
  // tuple<int,string,float,string,string,int,string.float,int&>!!!!!
  std::cout << "t2= " << t2 << std::endl;
  // Remember: tie ties references!
  n = 10;
  // The value of n has changed
  std::cout << "t2= " << t2 << std::endl;
}
