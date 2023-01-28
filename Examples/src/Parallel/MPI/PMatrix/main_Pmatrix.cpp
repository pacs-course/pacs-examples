/*
 * main_matrix.cpp
 *
 *  Created on: Oct 14, 2022
 *      Author: forma
 */
#include <iostream>
#include "PMatrix.hpp"
#include "chrono.hpp" // my chrono in Utilities
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop
int main()
{
  using namespace apsc::LinearAlgebra;
  MPI_Init(nullptr, nullptr);
  int mpi_rank;
  int mpi_size;
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  MPI_Comm_rank(mpi_comm, &mpi_rank);
  MPI_Comm_size(mpi_comm, &mpi_size);

  using RowMatrix=apsc::LinearAlgebra::Matrix<double,ORDERING::ROWMAJOR>;
  using ColMatrix=apsc::LinearAlgebra::Matrix<double,ORDERING::COLUMNMAJOR>;
  RowMatrix mr;

  std::size_t nRows=11;
  std::size_t nCols=20;
  if(mpi_rank==0)
    { std::cout<<"Creating a 11 by 20 Matrix of double, row-major\n";
    mr.resize(nRows,nCols);
    for (auto i=0u;i<mr.rows();++i)
      for (auto j=0u;j<mr.cols();++j)
        mr(i,j)=i+j;
    }
  //std::cout<<mr;
  apsc::PMatrix<RowMatrix> pmr;
  pmr.setup(mr,mpi_comm);
  int rank=0;
  while (rank < mpi_size)
    {
      if(mpi_rank==rank)
        {
          std::cout<<"Process rank="<< mpi_rank<<" Local Matrix=";
          std::cout<<pmr.getLocalMatrix();
       }
      rank++;
      MPI_Barrier(mpi_comm);
    }

  ColMatrix mc;
  if(mpi_rank==0)
     { std::cout<<"Creating a 11 by 20 Matrix of double, col-major\n";
     mc.resize(nRows,nCols);
     for (auto i=0u;i<mc.rows();++i)
       for (auto j=0u;j<mc.cols();++j)
         mc(i,j)=i+j;
     }

  apsc::PMatrix<ColMatrix> pmc;
  pmc.setup(mc,mpi_comm);
  rank=0;
  while (rank < mpi_size)
    {
      if(mpi_rank==rank)
        {
          std::cout<<"Process rank="<< mpi_rank<<" Local Matrix=";
          std::cout<<pmc.getLocalMatrix();
        }
      rank++;
      MPI_Barrier(mpi_comm);
    }

  std::vector<double> ones(nCols,1.);
  pmr.product(ones);
  // Only for debugging
// you need to make data public in PMatrix!
//  rank=0;
//  while (rank < mpi_size)
//    {
//      if(mpi_rank==rank)
//        {
//          std::cout<<"Process rank="<< mpi_rank<<" Local Product=";
//          for (auto i=0u;i<pmr.localProduct.size();++i)
//            std::cout<<pmr.localProduct[i]<<", ";
//          std::cout<<std::endl;
//        }
//      rank++;
//      MPI_Barrier(mpi_comm);
//    }


  std::vector<double> result;
  pmr.collectGlobal(result);
  if(mpi_rank==0)
    {
      std::cout<<"Testing row major version and all-to-one communication, residual should be 0\n";
      auto exact=mr*ones;
      double residual=0.0;
      for (auto i=0u;i<exact.size();++i)
        residual+=(exact[i]-result[i])*(exact[i]-result[i]);
      std::cout<<"Residual="<<residual<<std::endl;
    }


  // Now column major
  pmc.product(ones);
//   rank=0;
// Only for debugging
// you need to make data public in PMatrix!
//   while (rank < mpi_size)
//     {
//       if(mpi_rank==rank)
//         {
//           std::cout<<"Process rank="<< mpi_rank<<" Local Product=";
//           for (auto i=0u;i<pmc.localProduct.size();++i)
//             std::cout<<pmc.localProduct[i]<<", ";
//           std::cout<<std::endl;
//         }
//       rank++;
//       MPI_Barrier(mpi_comm);
//     }


   pmc.AllCollectGlobal(result);
   if(mpi_rank==mpi_size-1)
     {
       std::cout<<"Testing column major version and all-to-all communication, residual should be 0\n";
       auto exact=mr*ones;
       double residual=0.0;
       for (auto i=0u;i<exact.size();++i)
         residual+=(exact[i]-result[i])*(exact[i]-result[i]);
       std::cout<<"Residual="<<residual<<std::endl;
     }
   MPI_Barrier(mpi_comm);

   if(mpi_rank==0)
     std::cout<<"\nNow a more serious test"<<std::endl;
   Timings::Chrono clock;
   constexpr std::size_t N=1500;
   RowMatrix A;
   std::vector<double> uno(N,1.0);
   double time_scalar;
   double time_parallel;
   double time_setup;
   if(mpi_rank==0)
     {
       A.resize(N,N);
       A.fillRandom();// a random NxN matrix
       clock.start();
       result = A*uno;
       clock.stop();
       time_scalar=clock.wallTime();
       clock.start();
     }
   MPI_Barrier(mpi_comm);
   apsc::PMatrix<RowMatrix> Ap;
   Ap.setup(A, mpi_comm);
   if(mpi_rank==0)
     {
       clock.stop();
       time_setup=clock.wallTime();
       clock.start();
     }
   MPI_Barrier(mpi_comm);
   Ap.product(uno);
   Ap.collectGlobal(result);
   if(mpi_rank==0)
     {
       clock.stop();
       time_parallel=clock.wallTime();
       std::cout<<"Scalar time="<<time_scalar<<" Setup time="<<time_setup<<" Parallel product time="<<time_parallel<<std::endl;
       std::cout<<"Including setup: Speed-up="<<time_scalar/(time_setup+time_parallel)<<
           " Efficiency="<<(1./mpi_size)*time_scalar/(time_setup+time_parallel)<<std::endl;
       std::cout<<"Excluding setup: Speed-up="<<time_scalar/time_parallel<<
            " Efficiency="<<(1./mpi_size)*time_scalar/time_parallel<<std::endl;
     }



  //



  MPI_Finalize();



}


