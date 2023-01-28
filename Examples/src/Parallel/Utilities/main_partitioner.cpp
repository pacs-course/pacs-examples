/*
 * main_partitioner.cpp
 *
 *  Created on: Sep 17, 2022
 *      Author: forma
 */
#include "partitioner.hpp"
#include <iostream>
void printout(auto const & v)
{
  std::cout<<std::endl;
  for(auto const & e: v)
    {
      std::cout<<e<<", ";
    }
  std::cout<<std::endl;
}

int main()
{
  while(true)
    {
      std::cout<<"Give me number of elements and number of tasks (0,0 to end)\n";
      unsigned int num_elements;
      unsigned int num_tasks;
      std::cin>>num_elements>>num_tasks;
      if(num_elements*num_tasks==0) break;
      std::cout<<"min chunk size="<<num_elements/num_tasks<<", N. of larger chunks="<<num_elements % num_tasks<<std::endl;
      apsc::GroupedPartitioner grouped(num_tasks,num_elements);
      apsc::DistributedPartitioner distributed(num_tasks,num_elements);
      {
      auto [counts,displacements] = apsc::counts_and_displacements(grouped);
      std::cout<<"Number of elements in the chunks for the grouped strategy";
      printout(counts);
      }
      {
      auto [counts,displacements] = apsc::counts_and_displacements(distributed);
      std::cout<<"Number of elements in the chunks for the distributed strategy";
      printout(counts);
      }
    }

  std::cout<<"Now with a (row ordered) Matrix\n";
  while(true)
    {
      std::cout<<"Give me number of rows, columns and number of tasks (0,0 0 to end)\n";
      unsigned int num_rows;
      unsigned int num_cols;
      unsigned int num_tasks;
      std::cin>>num_rows>>num_cols>>num_tasks;
      if((num_rows*num_tasks*num_cols)==0u) return 0;

      apsc::MatrixPartitioner mpartitioner(num_rows,num_cols,num_tasks);
      for (auto t=0u;t<num_tasks;++t)
        {
          std::cout<<"Task "<<t<<" handles rows from "<<mpartitioner.first_row(t)<<"to "
              <<mpartitioner.last_row(t)-1<<". Data buffer locations: "<<mpartitioner.first(t)<<", "
              <<mpartitioner.last(t)-1<<std::endl;
        }
      auto [counts,displacements] = apsc::counts_and_displacements(mpartitioner);
      std::cout<<"Number of elements in the chunks:";
      printout(counts);
    }

}



