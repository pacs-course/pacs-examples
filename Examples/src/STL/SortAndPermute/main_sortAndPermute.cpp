/*
 * main_sortAndPermute.cpp
 *
 *  Created on: Mar 20, 2021
 *      Author: forma
 */
#include "SortAndPermute.hpp"
#include <vector>
#include <string>
#include <iostream>
int main()
{
  std::vector<int> x={1,3,2,9,6,-10};
  std::vector<std::string> s = {"one", "three", "two", "nine", "six","minus ten"};
  apsc::SortAndPermute sorter(x);
  std::cout<<"Original vector:\n";
  for (auto w:x) std::cout<<w<<", ";
  std::cout<<std::endl;
  auto permutation = sorter();
  std::cout<<"Permuted vector:\n";
  for (auto w:x) std::cout<<w<<", ";
  std::cout<<std::endl;
  std::cout<<"Permutation vector:\n";
  for (auto w:permutation) std::cout<<w<<", ";
  std::cout<<std::endl;
  // operate with permutation
  std::vector<std::string> new_s;
  for (auto i : permutation)
    new_s.push_back(s[i]);
  std::cout<<"Additional Permutated vector:\n";
   for (auto w:new_s) std::cout<<w<<", ";
   std::cout<<std::endl;

}



