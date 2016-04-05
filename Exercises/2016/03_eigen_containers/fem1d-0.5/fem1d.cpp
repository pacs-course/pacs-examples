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
  const unsigned int nnodes = cl.follow (100, 2, "-n", "-nnodes");

  mesh m (a, b, nnodes);
  Eigen::MatrixXd A(nnodes, nnodes);

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
              mloc(inode,jnode) = igrad * jgrad * m.h;
              A(m.elements[iel][inode],m.elements[iel][jnode]) += 
                mloc(inode,jnode);
            }
        }
    }

  Eigen::VectorXd f(nnodes);
  Eigen::Vector2d vloc;

  for (unsigned int iel = 0; iel < m.nels; ++iel)
    {
      vloc << 0, 0;
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          vloc(inode) = m.h / 2.0;
          f(m.elements[iel][inode]) += vloc(inode);
        }
    }

  f(0) = 0;
  f(nnodes - 1) = 0;

  A(0,0) = 1.0;
  A(nnodes-1,nnodes-1) = 1.0;
  for (unsigned int ii = 1; ii < nnodes; ++ii)
    {
      A(0, ii) = 0.0;
      A(nnodes-1, nnodes-1-ii) = 0.0;
    }

  
  Eigen::VectorXd uh = A.partialPivLu ().solve (f);

  for (unsigned int ii = 0; ii < nnodes; ++ii)
    std::cout << uh(ii, 0) << std:: endl;
      
  return 0;
};



