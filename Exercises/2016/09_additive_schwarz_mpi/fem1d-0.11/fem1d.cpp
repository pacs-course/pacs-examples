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
  const float a = cl.follow (float (0.0), "-a");
  const double b = cl.follow (float (1.0), "-b");
  const unsigned int nnodes = cl.follow (100, 2, "-n", "--nnodes");
  const std::string diffusion = cl.follow ("1.0", 2, "-d", "--diffusion");
  const std::string forcing = cl.follow ("1.0", 2, "-f", "--forcing");
  
  std::unique_ptr<mesh<double>> m (new mesh<double> (a, b, nnodes));
  fem_1d<double> problem (std::move (m));

  coeff<float> a_coeff (diffusion);
  problem.set_diffusion_coefficient (a_coeff);

  coeff<float> f_coeff (forcing);
  problem.set_source_coefficient (f_coeff);

  problem.assemble ();
  
  problem.set_dirichlet (fem_1d<double>::left_boundary, 0.0);
  problem.set_dirichlet (fem_1d<double>::right_boundary, 0.0);

  problem.solve ();
  
  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << problem.m->nodes[ii] << " "
              << problem.result ()(ii, 0)
              << std:: endl;
      
  return 0;
};
