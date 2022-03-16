/*
 * test_hash_combine.cpp
 *
 *  Created on: Mar 15, 2022
 *      Author: forma
 */
#include "hashCombine.hpp"
#include <iostream>
#include <string>

struct MyClass
{
  int         firstMember;
  double      secondMember;
  std::string thirdMember;
};

struct MyHash
{
  std::size_t
  operator()(MyClass const &x) const
  {
    std::size_t seed = 0u;
    apsc::hash_combine(seed, x.firstMember, x.secondMember, x.thirdMember);
    return seed;
  }
};

int
main()
{
  MyClass a{1, 10., "Hello"};
  MyClass b{-1, 7., "World"};
  MyClass c{99, 0., "Ciao"};
  MyClass d{1, -10., "Mondo"};
  MyHash  hash;
  std::cout << hash(a) << " " << hash(b) << " " << hash(c) << " " << hash(d)
            << std::endl;
}
