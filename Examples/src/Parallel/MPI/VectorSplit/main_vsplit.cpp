/*
 * main_vsplit.cpp
 *
 *  Created on: Aug 12, 2022
 *      Author: forma
 */
#include "utilities.hpp"
#include <iostream>
#include <mpi.h>
#include "GetPot"
#include "chrono.hpp"
void printHelp()
{
  std::cout<<"Splits a vector and compute some statistics in parallel\n";
  std::cout<<"Run the code as:\nmpirun -n p main_vsplit -size number_of_elements\n";
  std::cout<<"p: number of processes\nnumber_of_elements: the size of the vector (a big number, default:1e8)";
}

int main(int argc, char **argv)
{
  int mpi_rank,mpi_size;
  Timings::Chrono clock;
  MPI_Init(nullptr, nullptr);
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  MPI_Comm_rank(mpi_comm, &mpi_rank);
  MPI_Comm_size(mpi_comm, &mpi_size);
  int n;
  bool         stop{false};
  std::vector<int> count_recv(mpi_size);
  std::vector<int> displacements(mpi_size);
  std::vector<double> myVector;
  if(mpi_rank==0)
    {
      // get the data
      GetPot gp(argc,argv);
      if(gp.search(2, "-h", "--help"))
        {
          printHelp();
          stop = true;
        }
      n = gp.follow(100000000u, "-n");
      // compute partition
      auto chunk = n/mpi_size;
      auto rest  = n%mpi_size;
      for (auto i=0;i<mpi_size;++i)
        count_recv[i]= i<rest? chunk+1: chunk;
      displacements[0]=0;
      for (auto i=1;i<mpi_size;++i)
        displacements[i]=displacements[i-1]+count_recv[i-1];
    clock.start();
    }
  if(stop)
    {
      //MPI_Barrier(mpi_comm);
      MPI_Finalize();
      return 0;
    }
  // Scatter the chunks size across the processes
  int local_size;
  MPI_Scatter(count_recv.data(),1,MPI_INT, &local_size,1,MPI_INT,0,mpi_comm);
  // initialize local vector
  std::vector<double> local_v(local_size);
  // fill with random
  utilities::fill_random(local_v);
  auto local_mean=utilities::mean(local_v);
  auto local_minmax=utilities::minmax(local_v); // its an array of 2 doubles
  // Get in the root the full vector (an example of MPI_Gatherv)
  if (mpi_rank==0) myVector.resize(n);
  MPI_Gatherv(local_v.data(),local_size,MPI_DOUBLE,myVector.data(),count_recv.data(),
              displacements.data(),MPI_DOUBLE,0,mpi_comm);
  // Get also all partial statistics
  std::vector<double> all_means(mpi_size);
  std::vector<std::array<double,2>> all_minmax(mpi_size);
  MPI_Gather(&local_mean,1,MPI_DOUBLE,all_means.data(),1,MPI_DOUBLE,0,mpi_comm);
  MPI_Gather(local_minmax.data(),2,MPI_DOUBLE,all_minmax.data(),2,MPI_DOUBLE,0,mpi_comm);
  // final postprocessing to check
  if(mpi_rank==0)
    {
      auto millisec=clock.wallTimeNow();// time
      // compute the global mean and minmax from local computations
      double mean_from_local=0.;
      std::array<double,2> minmax_from_local={1.e50,-1.e50};
      for (auto i=0; i<mpi_size;++i)
        {
          mean_from_local+=all_means[i]*count_recv[i];
          minmax_from_local={std::min(minmax_from_local[0],all_minmax[i][0]),std::max(minmax_from_local[1],all_minmax[i][1])};
        }
      mean_from_local/=n;
      // for validation, compute the same quantities from the global values
      auto mean_from_global=utilities::mean(myVector);
      auto minmax_from_global=utilities::minmax(myVector);
      std::cout<<"Time taken: "<<millisec<<" milliseconds\n";
      std::cout<<"Local data:\n";
      for (auto i=0; i<mpi_size;++i)
        {
          std::cout<<"Process n."<<i<<" Mean:"<<all_means[i]<<"\tMin and max:"<<all_minmax[i][0]<<" "<<all_minmax[i][1]<<"\n";
        }
      std::cout<<"Global values from local data:\n";
      std::cout<<"Mean:"<<mean_from_local<<"\tMin and max:"<<minmax_from_local[0]<<" "<<minmax_from_local[1]<<std::endl;
      std::cout<<"Global values from global data:\n";
      std::cout<<"Mean:"<<mean_from_global<<"\tMin and max:"<<minmax_from_global[0]<<" "<<minmax_from_global[1]<<std::endl;
    }
  MPI_Finalize();
  return(0);
    }
