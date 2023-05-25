/*
 * main.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: forma
 */
#include <iostream>
#include <string>
#include <map>
#include <omp.h>
using MyMap=std::map<std::size_t, unsigned int>;

void add(MyMap & threadMap,unsigned int t, std::size_t i)
{
#pragma omp critical
  threadMap.insert({i,t});
}
void print(MyMap & threadMap)
{
  // to avoid garbling
for (auto v:threadMap)
  std::cout<<"Iteration n. "<<v.first<<" is performed by task  n."<<v.second<<std::endl;
threadMap.clear();
}

int main()
{
   int num_t=8;
   constexpr std::size_t n=20;
   MyMap threadMap;
   std::cout<<"Using "<<num_t<<" threads\n\n";

   std::cout<<"Static schedule (default)\n";
#pragma omp parallel for num_threads(num_t) schedule(static)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);

   std::cout<<"Static schedule with chunk 2\n";
#pragma omp parallel for num_threads(num_t) schedule(static, 2)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);

   std::cout<<"\nDynamic schedule\n";
#pragma omp parallel for num_threads(num_t) schedule(dynamic)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);


   std::cout<<"Dynamic schedule with chunk 2\n";
#pragma omp parallel for num_threads(num_t) schedule(dynamic, 2)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);


   std::cout<<"\nGuided schedule\n";
#pragma omp parallel for num_threads(num_t) schedule(guided)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);


   std::cout<<"Runtime schedule \n";
#pragma omp parallel for num_threads(num_t) schedule(runtime)
   for (std::size_t i=0u;i<n;++i)
     {
       add(threadMap,omp_get_thread_num(),i);
     }
   print(threadMap);




}


