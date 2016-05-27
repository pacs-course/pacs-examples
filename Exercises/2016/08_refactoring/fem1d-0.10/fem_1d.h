#ifndef HAVE_FEM_1D_H
#define HAVE_FEM_1D_H 

#include <algorithm>
#include <cmath>
#include <memory>
#include <array>
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <muParser.h>
#include "mesh.h"

std::string help_text = std::string () +
"fem1d command line options\n" +
  "-h, --help                       print this text and exit\n" +
  "-a <value>                       first end of interval\n" +
  "-b <value>                       second end of interval\n" +
  "-d, --diffusion \"string\"         expression to compute diffusion coefficient\n" +
  "-f, --forcing \"string\"           expression to compute forcing term coefficient\n" +
  "-n, --nnodes <value>             number of triangulation nodes\n" +
  "-q, --quadrature-rule \"string\"   name of the quadrature rule to use\n";


template<class prec = double>
class fem_1d
{

 private :

  Eigen::SparseMatrix<prec> A;  
  Eigen::Matrix<prec, 2, 2> mloc;
  Eigen::Matrix<prec, Eigen::Dynamic, 1> f;
  Eigen::Matrix<prec, 2, 1> vloc;
  Eigen::Matrix<prec, Eigen::Dynamic, 1> uh;
  std::function<prec (prec)> a_coeff;
  std::function<prec (prec)> f_coeff;
  
 public :

  const std::unique_ptr<mesh<prec>> m;
  enum bc_side {left_boundary, right_boundary};
  
  fem_1d (std::unique_ptr<mesh<prec>> _m) :
    m(std::move (_m))
  {
    A.resize (m->nnodes, m->nnodes);
    f.resize (m->nnodes);
    uh.resize (m->nnodes);
  };
  
  void
  assemble ();

  void
  solve ();

  void
  set_diffusion_coefficient (std::function<prec (prec)> _a) 
  { a_coeff = _a; };  

  void
  set_source_coefficient (std::function<prec (prec)> _f)
  { f_coeff = _f; };
  
  Eigen::SparseMatrix<prec> &
  matrix ()
  { return A; };

  Eigen::Matrix<prec, Eigen::Dynamic, 1> &
  rhs ()
  { return f; };

  Eigen::Matrix<prec, Eigen::Dynamic, 1> &
  result ()
  { return uh; };

  void
  set_dirichlet (fem_1d<prec>::bc_side, prec);
    
};

template<class prec>
void
fem_1d<prec>::assemble ()
{

  std::array<prec, 2> qn = {.0, 1.0};
  std::array<prec, 2> qw = {.5,  .5};

  A.setZero ();
  for (unsigned int iel = 0; iel < m->nels; ++iel)
      {
        mloc << 0, 0, 0, 0;      
        for (unsigned int inode = 0; inode < 2; ++inode)
          {
            double igrad = (inode == 0 ? 1.0 / m->h : -1.0 / m->h);
            for (unsigned int jnode = 0; jnode < 2; ++jnode)
              {
                double jgrad =  (jnode == 0 ? 1.0 / m->h : -1.0 / m->h);
                for (unsigned int qnode = 0; qnode < qn.size (); ++qnode)
                  mloc(inode,jnode) += igrad * jgrad * 
                    (a_coeff(m->nodes[m->elements[iel][0]] + m->h * qn[qnode]) *
                     m->h * qw[qnode]);
                
                A.coeffRef(m->elements[iel][inode],m->elements[iel][jnode]) += 
                  mloc(inode,jnode);
              }
          }
      }

  for (unsigned int iel = 0; iel < m->nels; ++iel)
    {
      vloc << 0, 0;
      
      for (unsigned int inode = 0; inode < 2; ++inode)
        {
          for (unsigned int qnode = 0; qnode < qn.size (); ++qnode)
            vloc(inode) +=
              (1.0 -
               fabs (m->nodes[m->elements[iel][inode]] -
                     (m->nodes[m->elements[iel][0]] +
                      m->h * qn[qnode])) / m->h ) *
              (qw[qnode] * m->h *
               f_coeff(m->nodes[m->elements[iel][0]] + m->h * qn[qnode]));

          f(m->elements[iel][inode]) += vloc(inode);
        }
    }

};

template<class prec>
void
fem_1d<prec>::set_dirichlet (const bc_side side, prec value)
{

  if (side ==  left_boundary)
    {
      f(0) = value;
      A.coeffRef(0,0) = prec (1.0);
      for (unsigned int ii = 1; ii < m->nnodes; ++ii)
        A.coeffRef(0, ii) = 0.0;
    }
  else if (side ==  right_boundary)
    {
      f(m->nnodes - 1) = value;
      A.coeffRef(m->nnodes-1, m->nnodes-1) = prec (1.0);
      for (unsigned int ii = 1; ii < m->nnodes; ++ii)
        A.coeffRef(m->nnodes-1, m->nnodes-1-ii) = 0.0;    
    }
};

template<class prec>
void
fem_1d<prec>::solve ()
{
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
  A.makeCompressed ();
  solver.analyzePattern(A); 
  solver.factorize(A);
  uh = solver.solve (f);
};

template<class prec>
class coeff
{
private :

  std::string expr;
  prec var;
  mu::Parser p;
  
public :

  coeff (const std::string & s) : expr (s)                          
  {
    try
      {
        p.DefineVar ("x", &var);
        p.SetExpr (expr.c_str ());
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg () << std::endl;
      }
  };

  prec operator() (prec x)
  {
    prec y;
    var = x;
    try
      {
        y = p.Eval ();
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg () << std::endl;
      }
    return (y);
  };
};

#endif


