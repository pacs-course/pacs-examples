/*
 * SaddlePointUtilities.hpp
 *
 *  Created on: May 19, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_

#include <iostream>
#include <fstream>
#include <filesystem>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include <Eigen/UmfPackSupport>
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include "MM_readers.hpp"
#include "preconditioner.hpp"
#include "BasicType.hpp"
#include "iml++.hpp"                          // All IML++
#include "GetPot"
#include <map>
#include <ios>
#include "chrono.hpp"
#include "preconditionerFactory.hpp"
#include "EigenSolver.hpp"
#include <string>
namespace SaddlePointUtilities
{
//! Select solvers
enum SolverSwitch {umfpack, gmres, gmresr, fgmres, minres, tminres,eigenvalues};
//enum PrecondSwitch {Diagonal, BlockDiagonal, BlockTriDiagonal, ILU, HH, Identity};
//! where to store parameters
struct TestParameters
{
  double tol;
  int max_iter;
  int gmres_levels;
  std::string MMatrixFileName;
  std::string BMatrixFileName;
  std::string TMatrixFileName;
  bool lumped=false;
  bool isSymUndef;
  unsigned int HSSMaxIter;
  double HSSalpha;
  double HSStol;
  SolverSwitch solverSwitch;
  FVCode3D::PrecondSwitch precondSwitch;
  std::string logFile;
  long int EIGncv=20L;
  long int EIGmaxit=1500L;
  double EIGtol=1.e-8;
};

//! A global variable that maps a string to the enum that indicates the solver
inline const std::map<std::string,SolverSwitch> getSolver=
    {
    {"umfpack",umfpack},
    {"gmres",gmres},
    {"gmresr",gmresr},
    {"fgmres",fgmres},
    {"minres",minres},
    {"tminres",tminres},
    {"eigenvalues",eigenvalues}
   };

//! A global variable that maps a string to the enum that indicates the solver
inline const std::map<std::string,FVCode3D::PrecondSwitch> getPreconditioner
{
  {"Diagonal", FVCode3D::Diagonal},
  {"BlockDiagonal",FVCode3D::BlockDiagonal},
  {"BlockTriangular",FVCode3D::BlockTriangular},
  {"ILU",FVCode3D::ILU},
  {"HSS",FVCode3D::HSS},
  {"Identity",FVCode3D::Identity},
  {"DoubleSaddlePoint",FVCode3D::DoubleSaddlePoint},
  {"DoubleSaddlePointSym",FVCode3D::DoubleSaddlePointSym}
};

//! Read parameters from a getpot object
inline TestParameters read_parameters(GetPot const & ifl)
{
  TestParameters r;
  r.tol            = ifl("tol", 1.0e-6);
  //std::cout<<" Tolerance ="<<r.tol<<std::endl;
  r.max_iter       = ifl("max_iter", 100);
  //std::cout<<" Max Iter ="<<r.max_iter<<std::endl;
  r.gmres_levels   = ifl("gmres_levels", 20);
  //std::cout<<" Gmres levels ="<<r.gmres_levels<<std::endl;
  r.MMatrixFileName = ifl("MMatrixname","NONE");
  r.BMatrixFileName = ifl("BMatrixname","NONE");
  r.TMatrixFileName = ifl("TMatrixname","NONE");
 if(r.MMatrixFileName == "NONE" || r.BMatrixFileName == "NONE" || r.TMatrixFileName == "NONE")
    {
      std::cerr<<" You have to give a good file name in the field\n";
      std::cerr<<" [M|B|T]MatrixFileName in the GetPot file\n";
      std::exit(1);
    }
   std::filesystem::path mcheck(r.MMatrixFileName);
   std::filesystem::path bcheck(r.BMatrixFileName);
   std::filesystem::path tcheck(r.TMatrixFileName);
   if(!mcheck.has_filename() || !bcheck.has_filename()||!tcheck.has_filename() )
     {
       std::cerr<<" You have to give a good file name in the field\n";
       std::cerr<<" [M|B|T]MatrixFileName in the GetPot file\n";
       std::cerr<<" One of the files is not existent\n";
       std::exit(1);
     }
  std::string  solver=ifl("solver","gmres");
  auto pos         = getSolver.find(solver); //:: is not necessary
  if (pos==getSolver.end())
    {
      std::cerr<<"Valid Solvers\n";
      for (auto [key,value]:getSolver)
	{
	  std::cerr<<key<<"\n";
	}
      throw std::runtime_error(std::string("FATAL ERROR: Solver ")+solver+std::string(" not found"));
    }
  r.solverSwitch   = pos->second;
  std::string  preconditioner=ifl("preconditioner","Identity");
  auto posp = getPreconditioner.find(preconditioner); //:: is not necessary
  if (posp==getPreconditioner.end())
    {
      std::cerr<<"Valid Preconditioners\n";
        for (auto [key,value]:getPreconditioner)
  	{
  	  std::cerr<<key<<"\n";
  	}
        throw std::runtime_error(std::string("FATAL ERROR: Precoditioner ")+preconditioner+std::string(" not found"));
    }
  r.precondSwitch   = posp->second;
  int test= ifl("isSymUndef",1);
  r.isSymUndef=(test!=0);
  test = ifl("isLumped",0);
  r.lumped=(test!=0);
  r.HSSMaxIter=ifl("HSS/max_iter",300);
  r.HSSalpha=ifl("HSS/alpha",1.e-2);
  r.HSStol=ifl("HSS/tol",1.e-2);
  r.logFile=ifl("logfile","NONE");
  // data for eigenvalues
  r.EIGncv = ifl("EIG/ncv",20L);
  r.EIGmaxit = ifl("EIG/maxit",1500L);
  r.EIGtol = ifl("EIG/tol",1.e-8);

  return r;
}
/*!
 * The name is misleading. It does not load anything. It sets the values needed by the
 * preconditioner
 * @param p the preconditioner
 * @param m The matrix to which the preconditioner will be applied
 * @param param The parameters
 */
inline void loadPreconditioner(FVCode3D::preconditioner & p, FVCode3D::SaddlePointMat const & m, TestParameters const & param)
{
  p.setLumping(param.lumped);
  p.set(m);
  p.set_alpha(param.HSSalpha);
  p.set_MaxIt(param.HSSMaxIter);
  p.set_tol(param.HSStol);
}
/*!
 * Writes the current value of the parameters
 * @param out an output stream
 * @param p The parameter
 * @param The output stream
 */
inline std::ostream & operator << (std::ostream & out, TestParameters const & p)
{
  {
    auto s   = p.solverSwitch;
    // get the iterator to the solver
    auto res=std::find_if(getSolver.cbegin(),getSolver.cend(),[s](auto const & i){return i.second==s;});
    out<<"solver= "<< res->first<<std::endl;
    if(s==gmres || s==gmresr || s==fgmres)
       out<<"gmres_levels= "<< p.gmres_levels<<std::endl;
    if(s==eigenvalues)
      out<<"EIGSOlver: maxiter="<<p.EIGmaxit<<" tolerance="<<p.EIGtol<<" Lanczos vectors="<<p.EIGncv<<std::endl;
  }
  {
    auto s   = p.precondSwitch;
    // get the iterator to the solver
    auto res=std::find_if(getPreconditioner.cbegin(),getPreconditioner.cend(),[s](auto const & i){return i.second==s;});
    out<<"preconditioner= "<< res->first<<std::endl;
    if (s==FVCode3D::HSS)
      {
	out<<"alpha="<<p.HSSalpha<<" cg tol="<<p.HSStol<<" cg max it="<<p.HSSMaxIter<<std::endl;
      }
  }
  out<<"tol= "<< p.tol<<std::endl;
  out<<"max_iter= "<< p.max_iter<<std::endl;
  out<<"M matrix file name= "<< p.MMatrixFileName<<std::endl;
  out<<"T matrix file name= "<< p.TMatrixFileName<<std::endl;
  out<<"B matrix file name= "<< p.BMatrixFileName<<std::endl;
  out<<"Symmetric indefinite form:"<<std::boolalpha<<p.isSymUndef<<" Lumping:"<<p.lumped<<std::endl;
  return out;
}
}// end namespace




#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_ */
