/*!
 * @file SaddlePointUtilities.hpp
 * @brief Runtime support for the saddle-point example.
 *
 * This header groups together:
 * - the parameter structure populated from a GetPot file,
 * - the string-to-enum maps used by the command driver,
 * - helper functions to configure the selected preconditioner.
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_

#include "BasicType.hpp"
#include "EigenSolver.hpp"
#include "GetPot"
#include "MM_readers.hpp"
#include "chrono.hpp"
#include "iml++.hpp" // All IML++
#include "preconditioner.hpp"
#include "preconditionerFactory.hpp"
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include <Eigen/UmfPackSupport>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
namespace SaddlePointUtilities
{
//! Solvers supported by `main.cpp`.
enum SolverSwitch
{
  umfpack,
  gmres,
  gmresr,
  fgmres,
  minres,
  tminres,
  eigenvalues
};
// enum PrecondSwitch {Diagonal, BlockDiagonal, BlockTriDiagonal, ILU, HH,
// Identity};
//! Parameters read from the GetPot configuration file.
/*!
 * The fields mirror the keys used in `data.pot` and are consumed by the main
 * program to select matrices, solver, preconditioner, and algorithm-specific
 * tolerances.
 */
struct TestParameters
{
  double                  tol;
  int                     max_iter;
  int                     gmres_levels;
  std::string             MMatrixFileName;
  std::string             BMatrixFileName;
  std::string             TMatrixFileName;
  bool                    lumped = false;
  bool                    isSymUndef;
  unsigned int            HSSMaxIter;
  double                  HSSalpha;
  double                  HSStol;
  SolverSwitch            solverSwitch;
  FVCode3D::PrecondSwitch precondSwitch;
  std::string             logFile;
  long int                EIGncv = 20L;
  long int                EIGmaxit = 1500L;
  double                  EIGtol = 1.e-8;
};

//! Map solver names as they appear in `data.pot` to runtime enums.
inline const std::map<std::string, SolverSwitch> getSolver = {
  {"umfpack", umfpack},        {"gmres", gmres},   {"gmresr", gmresr},
  {"fgmres", fgmres},          {"minres", minres}, {"tminres", tminres},
  {"eigenvalues", eigenvalues}};

//! Map preconditioner names as they appear in `data.pot` to runtime enums.
inline const std::map<std::string, FVCode3D::PrecondSwitch> getPreconditioner{
  {"Diagonal", FVCode3D::Diagonal},
  {"BlockDiagonal", FVCode3D::BlockDiagonal},
  {"BlockTriangular", FVCode3D::BlockTriangular},
  {"ILU", FVCode3D::ILU},
  {"HSS", FVCode3D::HSS},
  {"Identity", FVCode3D::Identity},
  {"DoubleSaddlePoint", FVCode3D::DoubleSaddlePoint},
  {"DoubleSaddlePointSym", FVCode3D::DoubleSaddlePointSym}};

//! Read and validate the configuration of a saddle-point test case.
/*!
 * @param ifl Parsed GetPot input file.
 * @return Fully populated parameter structure.
 * @throw std::runtime_error if the solver or preconditioner name is unknown.
 *
 * Matrix file names are also checked for basic consistency before the program
 * starts loading Matrix Market data.
 */
inline TestParameters
read_parameters(GetPot const &ifl)
{
  TestParameters r;
  r.tol = ifl("tol", 1.0e-6);
  // std::cout<<" Tolerance ="<<r.tol<<std::endl;
  r.max_iter = ifl("max_iter", 100);
  // std::cout<<" Max Iter ="<<r.max_iter<<std::endl;
  r.gmres_levels = ifl("gmres_levels", 20);
  // std::cout<<" Gmres levels ="<<r.gmres_levels<<std::endl;
  r.MMatrixFileName = ifl("MMatrixname", "NONE");
  r.BMatrixFileName = ifl("BMatrixname", "NONE");
  r.TMatrixFileName = ifl("TMatrixname", "NONE");
  if(r.MMatrixFileName == "NONE" || r.BMatrixFileName == "NONE" ||
     r.TMatrixFileName == "NONE")
    {
      std::cerr << " You have to give a good file name in the field\n";
      std::cerr << " [M|B|T]MatrixFileName in the GetPot file\n";
      std::exit(1);
    }
  std::filesystem::path mcheck(r.MMatrixFileName);
  std::filesystem::path bcheck(r.BMatrixFileName);
  std::filesystem::path tcheck(r.TMatrixFileName);
  if(!mcheck.has_filename() || !bcheck.has_filename() || !tcheck.has_filename())
    {
      std::cerr << " You have to give a good file name in the field\n";
      std::cerr << " [M|B|T]MatrixFileName in the GetPot file\n";
      std::cerr << " One of the files is not existent\n";
      std::exit(1);
    }
  std::string solver = ifl("solver", "gmres");
  auto        pos = getSolver.find(solver); //:: is not necessary
  if(pos == getSolver.end())
    {
      std::cerr << "Valid Solvers\n";
      for(auto [key, value] : getSolver)
        {
          std::cerr << key << "\n";
        }
      throw std::runtime_error(std::string("FATAL ERROR: Solver ") + solver +
                               std::string(" not found"));
    }
  r.solverSwitch = pos->second;
  std::string preconditioner = ifl("preconditioner", "Identity");
  auto posp = getPreconditioner.find(preconditioner); //:: is not necessary
  if(posp == getPreconditioner.end())
    {
      std::cerr << "Valid Preconditioners\n";
      for(auto [key, value] : getPreconditioner)
        {
          std::cerr << key << "\n";
        }
      throw std::runtime_error(std::string("FATAL ERROR: Precoditioner ") +
                               preconditioner + std::string(" not found"));
    }
  r.precondSwitch = posp->second;
  int test = ifl("isSymUndef", 1);
  r.isSymUndef = (test != 0);
  test = ifl("isLumped", 0);
  r.lumped = (test != 0);
  r.HSSMaxIter = ifl("HSS/max_iter", 300);
  r.HSSalpha = ifl("HSS/alpha", 1.e-2);
  r.HSStol = ifl("HSS/tol", 1.e-2);
  r.logFile = ifl("logfile", "NONE");
  // data for eigenvalues
  r.EIGncv = ifl("EIG/ncv", 20L);
  r.EIGmaxit = ifl("EIG/maxit", 1500L);
  r.EIGtol = ifl("EIG/tol", 1.e-8);

  return r;
}
//! Configure a preconditioner instance from the parsed input parameters.
/*!
 * The helper forwards the generic options shared by all preconditioners and
 * also passes the HSS-specific tuning parameters. For preconditioners that do
 * not use those values the setter hooks are harmless no-ops.
 *
 * @param p Preconditioner object selected at runtime.
 * @param m Saddle-point matrix to be preconditioned.
 * @param param Parsed parameter set.
 */
inline void
loadPreconditioner(FVCode3D::preconditioner &      p,
                   FVCode3D::SaddlePointMat const &m,
                   TestParameters const &          param)
{
  p.setLumping(param.lumped);
  p.set(m);
  p.set_alpha(param.HSSalpha);
  p.set_MaxIt(param.HSSMaxIter);
  p.set_tol(param.HSStol);
}
//! Pretty-print the effective runtime configuration.
/*!
 * This is used both on screen and on the optional log stream so each run keeps
 * a compact summary of the chosen matrices, solver, and preconditioner.
 *
 * @param out Output stream.
 * @param p Parameter set.
 * @return `out`.
 */
inline std::ostream &
operator<<(std::ostream &out, TestParameters const &p)
{
  {
    auto s = p.solverSwitch;
    // get the iterator to the solver
    auto res = std::find_if(getSolver.cbegin(), getSolver.cend(),
                            [s](auto const &i) { return i.second == s; });
    out << "solver= " << res->first << std::endl;
    if(s == gmres || s == gmresr || s == fgmres)
      out << "gmres_levels= " << p.gmres_levels << std::endl;
    if(s == eigenvalues)
      out << "EIGSOlver: maxiter=" << p.EIGmaxit << " tolerance=" << p.EIGtol
          << " Lanczos vectors=" << p.EIGncv << std::endl;
  }
  {
    auto s = p.precondSwitch;
    // get the iterator to the solver
    auto res =
      std::find_if(getPreconditioner.cbegin(), getPreconditioner.cend(),
                   [s](auto const &i) { return i.second == s; });
    out << "preconditioner= " << res->first << std::endl;
    if(s == FVCode3D::HSS)
      {
        out << "alpha=" << p.HSSalpha << " cg tol=" << p.HSStol
            << " cg max it=" << p.HSSMaxIter << std::endl;
      }
  }
  out << "tol= " << p.tol << std::endl;
  out << "max_iter= " << p.max_iter << std::endl;
  out << "M matrix file name= " << p.MMatrixFileName << std::endl;
  out << "T matrix file name= " << p.TMatrixFileName << std::endl;
  out << "B matrix file name= " << p.BMatrixFileName << std::endl;
  out << "Symmetric indefinite form:" << std::boolalpha << p.isSymUndef
      << " Lumping:" << p.lumped << std::endl;
  return out;
}
} // namespace SaddlePointUtilities

#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_SADDLEPOINTUTILITIES_HPP_ \
        */
