#include "fem_1d.h"
#include "GetPot"
  

int main (int argc, char **argv)
{

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

  const double tol = 1e-6;
  const unsigned int maxit = 43;
  const unsigned int overlap = 3;
  const double L = b - a;
  const int mpi_size = 3;

  const double L_loc = L / double (mpi_size);
  const double h = L_loc / ceil (double(nel) / double(mpi_size));

  std::vector<double> a_loc (mpi_size);
  std::vector<double> b_loc (mpi_size);
  std::vector<unsigned int> nel_loc (mpi_size);
  std::vector<unsigned int> ndof_loc (mpi_size);
  std::vector<fem_1d<double>*> subproblems (mpi_size);
  
  coeff<double> a_coeff (diffusion);
  coeff<double> f_coeff (forcing);

  int mpi_rank;
  for (mpi_rank = 0; mpi_rank < mpi_size; ++mpi_rank)
    {
      a_loc[mpi_rank] = a + mpi_rank * L_loc;
      b_loc[mpi_rank] = a_loc[mpi_rank] + L_loc;
      nel_loc[mpi_rank] = ceil (double(nel) / double(mpi_size));
      if (mpi_rank > 0)
        {
          a_loc[mpi_rank] -= overlap * h;
          nel_loc[mpi_rank] += overlap;
        }
      if (mpi_rank < mpi_size - 1)
        {
          b_loc[mpi_rank] += overlap * h;
          nel_loc[mpi_rank] += overlap;
        }
      ndof_loc[mpi_rank] = nel_loc[mpi_rank] + 1;
      fem_1d<double>* tmp = new fem_1d<double>(new mesh<double>
                                               (a_loc[mpi_rank],
                                                b_loc[mpi_rank],
                                                ndof_loc[mpi_rank]));
      subproblems[mpi_rank] = tmp;
      
      subproblems[mpi_rank]->set_diffusion_coefficient (a_coeff);
      subproblems[mpi_rank]->set_source_coefficient (f_coeff);
      subproblems[mpi_rank]->assemble ();
      subproblems[mpi_rank]->set_dirichlet (fem_1d<double>::left_boundary, 0.0);
      subproblems[mpi_rank]->set_dirichlet (fem_1d<double>::right_boundary, 0.0);
      subproblems[mpi_rank]->solve ();
    };

  for (unsigned int it = 0; it < maxit; ++it)
    for (mpi_rank = 0; mpi_rank < mpi_size; ++mpi_rank)
      {
        if (mpi_rank > 0)
          subproblems[mpi_rank]->set_dirichlet
            (fem_1d<double>::left_boundary,
             subproblems[mpi_rank-1]->result ()
             [ndof_loc[mpi_rank-1]-1-2*overlap]);
        else
          subproblems[mpi_rank]->set_dirichlet
            (fem_1d<double>::left_boundary, 0.0);
        if (mpi_rank < mpi_size - 1)
          subproblems[mpi_rank]->set_dirichlet
            (fem_1d<double>::right_boundary,
             subproblems[mpi_rank+1]->result ()
             [2*overlap]);
        else
          subproblems[mpi_rank]->set_dirichlet
            (fem_1d<double>::right_boundary, 0.0);
        subproblems[mpi_rank]->solve ();
      }

  for (mpi_rank = 0; mpi_rank < mpi_size; ++mpi_rank)
    for (unsigned int ii = 0; ii < ndof_loc[mpi_rank]; ++ii)
      std::cout << subproblems[mpi_rank]->m->nodes[ii] << " "
                << subproblems[mpi_rank]->result ()(ii, 0)
                << std::endl;

  return 0;
};
