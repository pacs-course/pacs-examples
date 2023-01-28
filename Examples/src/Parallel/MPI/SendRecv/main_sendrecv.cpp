/*
 * main_sendrecv.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: forma
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <mpi.h>
#include <VectorUtil.hpp>//namespace vectorUtil
#include "GetPot"
#include "chrono.hpp"
void printHelp()
{
  std::cout<<"Sorts a vector using an odd-even transposition algorithm for parallel sorting.\n";
  std::cout<<"Run the code as:\nmpirun -n p main_sendrecv -size number_of_elements\n";
  std::cout<<"p: number of processes\nnumber_of_elements: the size of the vector (a big number, default:1e8)\n";
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
      n = gp.follow(100000000u, "-size");
      myVector.resize(n);
      if(!stop)
        {
          vectorUtil::fill_random(myVector); // fill the vector with random numbers
          std::cout<<"Sorting a vector of size "<<n<<" with "<<mpi_size<<" processes\n";
        }
    }

  if(stop)
    {
      MPI_Barrier(mpi_comm);
      MPI_Finalize();
      return 0;
    }

  // Scatter the size across the processes
  MPI_Bcast(&n,1,MPI_INT,0,mpi_comm);
  // Build the communication pattern. I do it here and not just in the root process since I
  // need some of the data also locally and so it is cheaper
  // compute block partition directly
  // NOTE: I could have used the utility in Parallel/partioner.hpp
   auto chunk = n/mpi_size;
   auto rest  = n%mpi_size;
   for (auto i=0;i<mpi_size;++i)
     count_recv[i]= i<rest? chunk+1: chunk;
   displacements[0]=0;
   for (auto i=1;i<mpi_size;++i)
     displacements[i]=displacements[i-1]+count_recv[i-1];

  // initialize local vector
  int local_size=count_recv[mpi_rank];
  std::vector<double> local_v(local_size);
  // Get the local vector
  MPI_Scatterv(myVector.data(),count_recv.data(),displacements.data(), MPI_DOUBLE, local_v.data(),local_size,MPI_DOUBLE,0,mpi_comm);
  // root process may not need the full vector anymore, we can free memory. But here I keep it since I need it eventually
  /*
   if(mpi_rank==0)
   {
      myVector.clear();
      myVector.shrink_to_fit();
    }
    */
 // do the first local sorting
  vectorUtil::sort_vec_int(local_v);
  // I need mpi_size iterations to be sure to finish
  int partner;
  std::vector<double> partner_v;
  // I time only the more important part
  if(mpi_rank==0) clock.start();
  auto t1 = MPI_Wtime();
  if (mpi_size >0)
    {
      for (int phase =0;phase<mpi_size-1;++phase)
    {
      partner=vectorUtil::my_partner(mpi_rank, mpi_size, phase);
      // prepare the buffer to store the partner vector
      if(partner !=MPI_PROC_NULL) partner_v.resize(count_recv[partner]);
      // Receive and send data from/to partner
      MPI_Sendrecv(local_v.data(), local_v.size(), MPI_DOUBLE, partner, 0,
                   partner_v.data(), partner_v.size(), MPI_DOUBLE, partner,0,
                   mpi_comm, MPI_STATUS_IGNORE);
      // Now I need to keep only the good values, skip if I am idle
      if(partner !=MPI_PROC_NULL)
        {
          if(mpi_rank<partner) // keep lowest
            local_v=vectorUtil::merge<vectorUtil::LOW>(local_v, partner_v);
          else // keep highest
            local_v=vectorUtil::merge<vectorUtil::UP>(local_v, partner_v);
        }
    }
    }
// Gather final result in MyVector
  MPI_Gatherv(local_v.data(),local_v.size(),MPI_DOUBLE,myVector.data(),count_recv.data(), displacements.data(),MPI_DOUBLE,0,mpi_comm);
  // check if sorted
  if (mpi_rank ==0 )
  {
    clock.stop();
    auto t2 = MPI_Wtime();
    bool good{true};
    for (auto i =0u; i<myVector.size()-1;++i)
      {
     good = good && (myVector[i]<=myVector[i+1]);
      }
    std::string message{"successful"};
    if(!good) message = "NOT SUCCESSFUL";
    std::cout<<"Sorted in "<<std::setprecision(6)<<clock.wallTime()<<" microsec. with Linux clock, "<<t2-t1<<" sec with MPI time\n";
    std::cout<<"Sorting "<<message<<std::endl;
  }
   MPI_Finalize();
   return 0;
}

