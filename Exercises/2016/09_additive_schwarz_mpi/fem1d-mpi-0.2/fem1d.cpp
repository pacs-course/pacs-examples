#include "mpi.h"
#include "fem_1d.h"
#include "GetPot"
  

int main (int argc, char **argv)
{


  MPI_Init (&argc, &argv);
    
  GetPot cl (argc, argv);

  if (cl.search (2, "-h", "--help"))
    {
      std::cerr << help_text << std::endl;
      return 0;
    }
  
  const double a = cl.follow (double (0.0), "-a");
  const double b = cl.follow (double (1.0), "-b");
  const unsigned int nnodes = cl.follow (100, 2, "-n", "--nnodes");
  const unsigned int nel = nnodes - 1;
  const std::string diffusion = cl.follow ("1.0", 2, "-d", "--diffusion");
  const std::string forcing = cl.follow ("1.0", 2, "-f", "--forcing");
  const double L = b - a;

  constexpr double tol = 1e-6;
  constexpr unsigned int maxit = 100;
  constexpr unsigned int overlap = 100;

  MPI_Status status;
  MPI_Request request1, request2;
  
  int mpi_size, mpi_rank, tag;
  MPI_Comm_size (MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank (MPI_COMM_WORLD, &mpi_rank);
  
  const double L_loc = L / double(mpi_size);
  const double h = L_loc / ceil (double(nel) / double(mpi_size));

  double a_loc  = .0;
  double lval   = .0;
  double b_loc  = .0;
  double rval   = .0;
  double buffer = .0;
  
  unsigned int nel_loc = 0;
  unsigned int ndof_loc = 1;
  fem_1d<double> *subproblems;
  
  coeff<double> a_coeff (diffusion);
  coeff<double> f_coeff (forcing);


  a_loc = a + mpi_rank * L_loc;
  b_loc = a_loc + L_loc;
  nel_loc = ceil (double(nel) / double(mpi_size));
  if (mpi_rank > 0)
    {
      a_loc -= overlap * h;
      nel_loc += overlap;
    }
  if (mpi_rank < mpi_size - 1)
    {
      b_loc += overlap * h;
      nel_loc += overlap;
    }
  ndof_loc = nel_loc + 1;
  subproblems = new fem_1d<double>
    (new mesh<double> (a_loc, b_loc, ndof_loc));
      
  subproblems->set_diffusion_coefficient (a_coeff);
  subproblems->set_source_coefficient (f_coeff);
  subproblems->assemble ();
  subproblems->set_dirichlet (fem_1d<double>::left_boundary, 0.0);
  subproblems->set_dirichlet (fem_1d<double>::right_boundary, 0.0);
  subproblems->solve ();

  for (unsigned int it = 0; it < maxit; ++it)
    {

      // With the following implementation
      // communication will occur in
      // unpredictable order

      tag = 10*mpi_rank;
      // Send to right neighbour
      if (mpi_rank < mpi_size - 1)
        {
          buffer = subproblems->result ()
            [ndof_loc - 1 - 2*overlap];

          std::cerr << "rank " << mpi_rank
                    << " sending lval to rank "
                    << mpi_rank + 1
                    << std::endl;

          MPI_Isend (&buffer, 1, MPI_DOUBLE, mpi_rank + 1, tag,
                     MPI_COMM_WORLD, &request1);

        }

      tag = 10*mpi_rank + 1;
      // Send to right neighbour
      if (mpi_rank > 0)
        {
          buffer = subproblems->result () [2*overlap];

          std::cerr << "rank " << mpi_rank
                    << " sending rval to rank "
                    << mpi_rank - 1
                    << std::endl;

          MPI_Isend (&buffer, 1, MPI_DOUBLE, mpi_rank - 1, tag,
                     MPI_COMM_WORLD, &request2);

        }

      
      // Receive from left neighbour
      if (mpi_rank > 0)
        {

          std::cerr << "rank " << mpi_rank
                    << " receiving lval from rank "
                    << mpi_rank - 1
                    << std::endl;

          MPI_Recv (&buffer, 1, MPI_DOUBLE, mpi_rank - 1, MPI_ANY_TAG,
                    MPI_COMM_WORLD, &status);

          std::cerr << "rank " << mpi_rank
                    << " received lval from rank "
                    << mpi_rank - 1
                    << std::endl;
            
          lval = buffer;
        }


      // Receive from right neighbour
      if (mpi_rank < mpi_size - 1)
        {
          std::cerr << "rank " << mpi_rank
                    << " receiving rval from rank "
                    << mpi_rank + 1
                    << std::endl;
          
          MPI_Recv (&buffer, 1, MPI_DOUBLE, mpi_rank + 1, MPI_ANY_TAG,
                    MPI_COMM_WORLD, &status);

          std::cerr << "rank " << mpi_rank
                    << " received rval from rank "
                    << mpi_rank + 1
                    << std::endl;

          rval = buffer;
        }

      if (mpi_rank > 0)
        MPI_Wait(&request2, &status);

      if (mpi_rank < mpi_size - 1)
        MPI_Wait(&request1, &status);
            
      subproblems->set_dirichlet
        (fem_1d<double>::left_boundary, lval);
      
      subproblems->set_dirichlet
        (fem_1d<double>::right_boundary, rval);
      
      subproblems->solve ();

    }

  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (rank == mpi_rank)
        for (unsigned int ii = 0; ii < ndof_loc; ++ii)
          std::cout << subproblems->m->nodes[ii] << " "
                    << subproblems->result ()(ii, 0)
                    << std::endl;
      MPI_Barrier (MPI_COMM_WORLD);
    }

  MPI_Finalize ();
  return 0;

};
