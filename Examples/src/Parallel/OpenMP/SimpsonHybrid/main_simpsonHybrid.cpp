/*
 * main_simpsonMPI.cpp
 *
 *  Created on: Aug 11, 2022
 *      Author: forma
 */
#include "chrono.hpp"
#include "json.hpp"
#include "mpi.h"
#include <cmath> // for sin
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "../SimpsonHybrid/SimpsonRule.hpp"
int
main()
{
  int             my_rank, comm_sz;
  Timings::Chrono clock;
  // This tuple is here to pack some data
  std::tuple<double, double, double, unsigned int, unsigned int> databuf;
  // I use a function wrapper to wrap the function
  std::function<double(double const &)> f = [](double const &x) {
    return std::sin(x) * cos(20 * x);
  };
  MPI_Init(nullptr, nullptr);
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  MPI_Comm_rank(mpi_comm, &my_rank);
  MPI_Comm_size(mpi_comm, &comm_sz);
  // it will store the number of elements in each process
  std::vector<unsigned int> n_local_v(comm_sz);
  // it will store the starting point of the subintervl treated by each proc.
  std::vector<double> a_local_v(comm_sz);
  // root process does some preprocessing
  if(my_rank == 0)
    {
      // Opening the file with data.
      // Read from a file
      std::ifstream ifile("data.json"); // open file
      using json = nlohmann::json;      // to make life easier
      json j1;                          // create a json object
      ifile >> j1;                      // load the file in the json object
      ifile.close();                    // close file
      // Extract data. you need to use get<type> to select the type
      // Alternatively, you may use the utility value
      auto a = j1["a"].get<double>(); // get data from json object
      auto b = j1["b"].get<double>();
      auto n = j1["n"].get<unsigned int>();
      auto num_threads = j1.value("num_threads", 1u);
      std::cout << "Computing Simpson composite rule between " << a << " and "
                << b << " with " << n << " intervals\n";
      std::cout << "Using " << comm_sz << " MPI processes and " << num_threads
                << " threads\n";

      double h = (b - a) / n;
      databuf =
        std::tie(a, b, h, n,
                 num_threads); // To pack some data (only to show hot it works)
      // split the subintervals
      auto n_local_base = n / comm_sz;
      int  rest = n % comm_sz; // maybe n is not a multiple of comm_sz
      a_local_v[0] = a;
      for(auto i = 0; i < comm_sz; ++i)
        {
          n_local_v[i] = i < rest ? n_local_base + 1 : n_local_base;
        }
      for(auto i = 1; i < comm_sz; ++i)
        {
          a_local_v[i] = a_local_v[i - 1] + h * n_local_v[i - 1];
        }
      clock.start(); // start timings
    }
  // Broadcast, we use a trick to send some data in one message and avoid the
  // use of MPI_Pack
  MPI_Bcast(&databuf, sizeof(databuf), MPI_BYTE, 0, mpi_comm);
  auto [a, b, h, n, num_threads] =
    databuf; // get global data (in fact we need only h)
  unsigned int n_local;
  double       a_local;
  // Scatter the local parameters to all processes
  MPI_Scatter(n_local_v.data(), 1, MPI_UNSIGNED, &n_local, 1, MPI_UNSIGNED, 0,
              mpi_comm);
  MPI_Scatter(a_local_v.data(), 1, MPI_DOUBLE, &a_local, 1, MPI_DOUBLE, 0,
              mpi_comm);
  auto b_local = a_local + h * n_local;
  std::cout << "Process " << my_rank << " a_local, b_local, n_local=" << a_local
            << " " << b_local << " " << n_local << std::endl;
  double local_int = SimpsonRule_mt(f, a_local, b_local, n_local, num_threads);
  double integral{0.};
  MPI_Reduce(&local_int, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if(my_rank == 0)
    {
      std::cout << "Integral value " << std::setprecision(16) << integral
                << " Computed in " << clock.wallTimeNow() << " microsec.\n";
    }
  MPI_Finalize();
  return 0;
}
