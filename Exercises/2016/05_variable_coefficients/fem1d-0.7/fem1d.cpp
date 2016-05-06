#include "fem1d.h"
#include "mesh.h"
#include "GetPot"
  

int main (int argc, char **argv)
{

  GetPot cl (argc, argv);

  if (cl.search (2, "-h", "--help"))
    {
      std::cerr << help_text << std::endl;
      return 0;
    }
  const double a = cl.follow (0.0, "-a");
  const double b = cl.follow (1.0, "-b");
  const unsigned int nnodes = cl.follow (100, 2, "-n", "--nnodes");
  const std::string diffusion = cl.follow ("1.0", 2, "-d", "--diffusion");
  const std::string forcing = cl.follow ("1.0", 2, "-f", "--forcing");
  
  coeff f_coeff (forcing);
  coeff a_coeff (diffusion);

  std::vector<double> qn;
  std::vector<double> qw;
  const std::string rule = cl.follow ("trapezoidal", 2, "-q", "--quadrature-rule");

  
  if (rule == "trapezoidal")
    {
      qn.push_back (0.0);
      qn.push_back (1.0);
      qw.push_back (0.5);
      qw.push_back (0.5);
    }
  else if (rule == "midpoint")
    {
      qn.push_back (0.5);
      qw.push_back (1.0);
    }
  else
    std::cerr << "unknown quadrature rule" << std::endl;

  
  mesh m (a, b, nnodes);
  Eigen::SparseMatrix<double> A(nnodes, nnodes);
  
  Eigen::Matrix2d mloc;
  mloc << 0, 0, 0, 0;
  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {

      mloc << 0, 0, 0, 0;      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          double igrad = (inode == 0 ? 1.0 / m.h : -1.0 / m.h);
          for (unsigned int jnode = 0; jnode < 2; ++jnode)
            {
              double jgrad =  (jnode == 0 ? 1.0 / m.h : -1.0 / m.h);
              for (unsigned int qnode = 0; qnode < qn.size (); ++qnode)
                mloc(inode,jnode) += igrad * jgrad * 
                  (a_coeff(m.nodes[m.elements[iel][0]] + m.h * qn[qnode]) *
                   m.h * qw[qnode]);
              
              A.coeffRef(m.elements[iel][inode],m.elements[iel][jnode]) += 
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
          for (unsigned int qnode = 0; qnode < qn.size (); ++qnode)
            vloc(inode) +=
              (1.0 -
               fabs (m.nodes[m.elements[iel][inode]] -
                     (m.nodes[m.elements[iel][0]] +
                      m.h * qn[qnode])) / m.h ) *
              (qw[qnode] * m.h *
               f_coeff(m.nodes[m.elements[iel][0]] + m.h * qn[qnode]));

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
    std::cout << m.nodes[ii] << " " << uh(ii, 0) << std:: endl;
      
  return 0;
};
