#include "fem1d.h"
#include "mesh.h"
#include "GetPot"
#include <dlfcn.h>

double
basisfun (double x, double a, double b,
          double phia, double phib)
{
  double retval = 0.0;
  retval = phia * (x - b) / (a - b) +
    phib * (x - a) / (b - a);
}

double
basisfungrad (double x, double a, double b,
              double phia, double phib)
{
  double retval = 0.0;
  retval = phia / (a - b) +
    phib / (b - a);
}

int main (int argc, char **argv)
{

  GetPot cl (argc, argv);

  if (cl.search (2, "-h", "--help"))
    {
      std::cerr << help_text << std::endl;
      return 0;
    }

  const double a =
    cl.follow (0.0, "-a");

  const double b =
    cl.follow (1.0, "-b");

  const unsigned int nnodes =
    cl.follow (100, 2, "-n", "--nnodes");

  const std::string diffusion =
    cl.follow ("1.0", 2, "-d", "--diffusion");

  const std::string forcing =
    cl.follow ("1.0", 2, "-f", "--forcing");
  
  coeff f_coeff (forcing);
  coeff a_coeff (diffusion);

  const std::string quadrature =
    cl.follow ("adaptive_quadrature.so",
               2, "-q", "--quadrature-rule");

  std::function
    <double (std::function<double (double)>, double, double)>
    integrate;
  
  void * handle = dlopen (quadrature.c_str (), RTLD_LAZY);
  if (! handle)
    {
      std::cerr << "fem1d: cannot load dynamic object!"
                << std::endl;
      std::cerr << dlerror ()
                << std::endl;
      return (-1);
    }

  void * sym = dlsym (handle, "integrate");
  if (! sym)
    {
      std::cerr << "fem1d: cannot load symbol!"
                << std::endl;
      std::cerr << dlerror ()
                << std::endl;
      return (-1);
    }
  
  integrate =
    reinterpret_cast
    <double (*) (std::function<double (double)>, double, double)>
    (sym);
  
  mesh m (a, b, nnodes);
  Eigen::SparseMatrix<double> A(nnodes, nnodes);
  
  Eigen::Matrix2d mloc;
  mloc << 0, 0, 0, 0;
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {

      mloc << 0, 0, 0, 0;      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {

          auto igrad = [=, &m] (double x) -> double
            {
              return basisfungrad
              (x, m.nodes[m.elements[iel][0]],
               m.nodes[m.elements[iel][1]],
               inode == 0 ? 1.0 : 0.0, 
               inode == 1 ? 1.0 : 0.0);
            };

          
          for (unsigned int jnode = 0; jnode < 2; ++jnode)
            {

              auto jgrad = [=, &m] (double x) -> double
                {
                  return basisfungrad
                  (x, m.nodes[m.elements[iel][0]],
                   m.nodes[m.elements[iel][1]],
                   jnode == 0 ? 1.0 : 0.0, 
                   jnode == 1 ? 1.0 : 0.0);
                };
              
              mloc(inode,jnode) +=
                integrate ([=, &m, &igrad, &jgrad, &a_coeff]
                           (double x) -> double
                           {
                             return (igrad (x) *
                                     jgrad (x) *
                                     a_coeff (x));
                           },
                           m.nodes[m.elements[iel][0]],
                           m.nodes[m.elements[iel][1]]);

              
              A.coeffRef(m.elements[iel][inode],
                         m.elements[iel][jnode]) += 
                mloc(inode,jnode);
              
            }
        }
    }

  Eigen::VectorXd f(nnodes);
  for (unsigned int ii = 0; ii < nnodes; ++ii)
    f(ii) = 0.0;
    
  Eigen::Vector2d vloc;

  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {
      vloc << 0, 0;
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {

          auto ifun = [=, &m] (double x) -> double
            {
              return basisfun
              (x, m.nodes[m.elements[iel][0]],
               m.nodes[m.elements[iel][1]],
               inode == 0 ? 1.0 : 0.0,
               inode == 1 ? 1.0 : 0.0);
            };
          
            vloc(inode) +=
              integrate ([=, &m, &ifun, &f_coeff]
                         (double x) -> double
                         {
                           return (ifun (x) *
                                   f_coeff (x));
                         },
                         m.nodes[m.elements[iel][0]],
                         m.nodes[m.elements[iel][1]]);

              
          f(m.elements[iel][inode]) += vloc(inode);
        }
    }

  f(0) = 0;
  f(nnodes - 1) = 0;

  A.coeffRef(0,0) = 1.0e10;
  A.coeffRef(nnodes-1,nnodes-1) = 1.0e10;
  for (unsigned int ii = 1; ii < nnodes; ++ii)
    {
      A.coeffRef(0, ii) = 0.0;
      A.coeffRef(nnodes-1, nnodes-1-ii) = 0.0;
    }
    
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
  A.makeCompressed ();
  solver.analyzePattern(A); 
  solver.factorize(A);
  
  Eigen::VectorXd uh = solver.solve (f);

  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << m.nodes[ii] << " "
              << uh(ii, 0)
              << std:: endl;
      
  return 0;
};
