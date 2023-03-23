/*
 * main_sortAndPermute.cpp
 *
 *  Created on: Mar 20, 2021
 *      Author: forma
 */
#include "SortAndPermute.hpp"
#include <iostream>
#include <string>
#include <vector>
int
main()
{
  std::vector<int>         x = {1, 3, 2, 9, 6, -10};
  std::vector<std::string> s = {"one",  "three", "two",
                                "nine", "six",   "minus ten"};
  apsc::SortAndPermute<std::vector<int>>     sorter(x);

  std::cout << "Original vector:\n";
  for(auto w : x)
    std::cout << w << ", ";
  std::cout << std::endl;
  sorter(); // sort
  std::cout << "Permuted vector:\n";
  for(auto w : sorter.data())
    std::cout << w << ", ";

  std::cout << std::endl;
  std::cout << "Permutation vector:\n";
  for(auto w : sorter.permutation())
    std::cout << w << ", ";
  std::cout << std::endl;

  // operate with permutation
  std::cout<<std::endl;
  std::cout << "Original vector:\n";
   for(auto w : s)
     std::cout << w << ", ";
   std::cout << std::endl;
  std::cout << "Permutated vector according to given permutation:\n";
  for(auto i : apsc::applyPermutation(s,sorter.permutation()))
    std::cout<<i<<", ";
  std::cout<<std::endl;
  std::cout << "Permutated vector in place:\n";
  apsc::applyPermutationInPlace(s,sorter.permutation());
  for(auto w : s)
    std::cout << w << ", ";
  std::cout << std::endl;
  auto ip=apsc::invertPermutation(sorter.permutation());
  std::cout << "Back to the original:\n";
  apsc::applyPermutationInPlace(s,ip);
  for(auto w : s)
    std::cout << w << ", ";
  std::cout << std::endl;

  // operate on strings
   std::cout<<std::endl;
   std::cout << "Just for fun, let's work with stings\nOriginal vector:\n";
    for(auto w : s)
      std::cout << w << ", ";
    std::cout << std::endl;
    ip=apsc::sortAndPermute(s);
    std::cout << "Permuted vector:\n";
    for(auto w : s)
      std::cout << w << ", ";

    std::cout << std::endl;
    std::cout << "Permutation vector:\n";
    for(auto w : ip)
      std::cout << w << ", ";
    std::cout << std::endl;

}
