/*
 * main_lock.cpp
 *
 *  Created on: Aug 25, 2022
 *      Author: forma
 */
#include <iostream>
#include <vector>
#include "LockClasses.hpp"
int main()
{
  std::vector<NoLocked> v_nl(5); // 5 elements
  std::vector<Locked> v_l(5); // 5 elements
  std::vector<std::size_t> indexes={0,0,1,0,2,2,4,4,3,0,1,3,4,1,1};
  std::vector<int> rightValues={4,2,2,2,3};
  std::cout<<"Wrong code, I am not using locks\n";
#pragma omp parallel for num_threads(8) shared(v_nl)
  for (auto i:indexes)
    {
      v_nl[i].update_c();
    }

  std::cout<<"Value of counters:";
      for (auto v:v_nl)
        std::cout<<v.howMany()<<", ";
  std::cout<<std::endl;

  std::cout<<"Correct values   :";
       for (auto v:rightValues)
         std::cout<<v<<", ";
   std::cout<<std::endl;

   std::cout<<"Correct code, I am  using locks\n";
 #pragma omp parallel for num_threads(8) shared(v_l)
   for (auto i:indexes)
     {
       v_l[i].update_c();
     }

   std::cout<<"Value of counters:";
       for (auto v:v_l)
         std::cout<<v.howMany()<<", ";
   std::cout<<std::endl;

   std::cout<<"Correct values   :";
        for (auto v:rightValues)
          std::cout<<v<<", ";
    std::cout<<std::endl;


}


