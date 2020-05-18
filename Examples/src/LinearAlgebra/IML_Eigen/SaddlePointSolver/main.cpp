#include <stdlib.h>                         // System includes
//#include <iostream.h>                       //                   //
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
};

//! Map solvers to switch
const std::map<std::string,SolverSwitch> getSolver=
    {
    {"umfpack",umfpack},
    {"gmres",gmres},
    {"gmresr",gmresr},
    {"fgmres",fgmres},
    {"minres",minres},
    {"tminres",tminres},
    {"eigenvalues",eigenvalues}
   };

const std::map<std::string,FVCode3D::PrecondSwitch> getPreconditioner
{
  {"Diagonal", FVCode3D::Diagonal},
  {"BlockDiagonal",FVCode3D::BlockDiagonal},
  {"BlockTriangular",FVCode3D::BlockTriangular},
  {"ILU",FVCode3D::ILU},
  {"HSS",FVCode3D::HSS},
  {"Identity",FVCode3D::Identity},
};

//! Read parameters from getpot object
TestParameters read_parameters(GetPot const & ifl)
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
  auto pos         = ::getSolver.find(solver); //:: is not necessary
  if (pos==::getSolver.end())
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
  auto posp = ::getPreconditioner.find(preconditioner); //:: is not necessary
  if (posp==::getPreconditioner.end())
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
  return r;
}
// It is useful to write the parameters just to check

void loadPreconditioner(FVCode3D::preconditioner & p, FVCode3D::SaddlePointMat const & m, TestParameters const & param)
{
  p.set(m);
  p.setLumping(param.lumped);
  p.set_alpha(param.HSSalpha);
  p.set_MaxIt(param.HSSMaxIter);
  p.set_tol(param.HSStol);
}

std::ostream & operator << (std::ostream & out, TestParameters const & p)
{
  {
    auto s   = p.solverSwitch;
    // get the iterator to the solver
    auto res=std::find_if(getSolver.cbegin(),getSolver.cend(),[s](auto const & i){return i.second==s;});
    out<<"solver= "<< res->first<<std::endl;
    if(s==gmres || s==gmresr || s==fgmres)
       out<<"gmres_levels= "<< p.gmres_levels<<std::endl;
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
  out<<"Symmetric indefinite form:"<<std::boolalpha<<p.isSymUndef<<std::endl;
  return out;
}

int
main(int argc, char * argv[])
{
  using std::endl;
  using std::cin;
  using std::cout;
  using std::cerr;
  using namespace LinearAlgebra;
  // Parse command line with getpot
   GetPot indata(argc,argv);
  if ( indata.search(2,"--help","-h") )
    {
      cout << "Usage: " << argv[0] << "<-i DataFile>" << endl;
      cout << "DataFile is a GetPot file. If missing is set to data.pot" << endl;
      return 0;
    };
  std::string dataFile;
  dataFile=indata.follow("./data.pot","-i");
  std::cout<<" Reading data from "<<dataFile<<std::endl;
  std::filesystem::path fpath(dataFile);
  if(!fpath.has_filename())
    {
      std::cerr<<"Getpot file not present\n";
      return 1;
    }
  std::filesystem::path apath{std::filesystem::absolute(fpath)};
  // Read parameters from data
  GetPot ifl(apath.c_str());
  TestParameters testParameters=read_parameters(ifl);
  // Output parameters
  std::cout<<testParameters<<std::endl;
  std::ofstream logger;
  if (testParameters.logFile != "NONE")
    {
      std::cout<<"Logging in "<<testParameters.logFile<<std::endl;
      logger.open(testParameters.logFile);
      // redirect logging
      std::clog.rdbuf(logger.rdbuf());
      std::clog<<testParameters<<std::endl;
    }
  // Some useful alias
  // Matrix is in column ordering
  using SpMat=FVCode3D::SpMat;
  using SpVec=FVCode3D::Vector;

  // Read matrices
  std::string matrixFile;
  matrixFile=testParameters.MMatrixFileName;
  SpMat M=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  std::clog<<"M Matrix size:"<<M.rows()<<"X"<<M.cols()<<std::endl;
  std::clog<<"Non zero entries:"<<M.nonZeros()<<std::endl;

  matrixFile=testParameters.BMatrixFileName;
  SpMat B=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  std::clog<<"B Matrix size:"<<B.rows()<<"X"<<B.cols()<<std::endl;
  std::clog<<"Non zero entries:"<<B.nonZeros()<<std::endl;

  matrixFile=testParameters.TMatrixFileName;
  SpMat T=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  std::clog<<"T Matrix size:"<<T.rows()<<"X"<<T.cols()<<std::endl;
  std::clog<<"Non zero entries:"<<T.nonZeros()<<std::endl;
// assign to saddle point matrix
  FVCode3D::SaddlePointMat saddlePointMat(M,B,T,testParameters.isSymUndef);
// The saddlepoint matrix is composed with the block matrices (to be changed)
// free memory
  if (testParameters.solverSwitch==eigenvalues)
    {
      Eigen::VectorXd evalues=computeSymEigenValues(M,2);
      std::clog<<"Eigenvalues of M "<<evalues<<std::endl;
      std::clog<<"Condition Number of M "<<evalues(0)/evalues(evalues.size()-1)<<std::endl;
                   evalues=computeSymEigenValues(T,2);
      std::clog<<"Eigenvalues of T "<<evalues<<std::endl;
      auto nn=saddlePointMat.squaredNorm();
      std::clog<<"Norm of SPM "<<std::sqrt(nn)<<" Normalised norm "<<std::sqrt(nn/saddlePointMat.nonZeros())<<std::endl;
      return 0;
    }
  M.resize(0,0);
  B.resize(0,0);
  T.resize(0,0);
  M.data().squeeze();
  B.data().squeeze();
  T.data().squeeze();


  // Just to make sure it is in a compressed mode
  saddlePointMat.makeCompressed();
  // Create the right hand side so that the solution is a vector of 1s
  //
  SpVec e=SpVec::Ones(saddlePointMat.getM().rows()+saddlePointMat.getB().rows());
  SpVec b=saddlePointMat * e;
  SpVec x(saddlePointMat.getM().rows()+saddlePointMat.getB().rows());
  x.fill(0.);// Start from 0.
  FVCode3D::PreconditionerFactory factory= FVCode3D::make_PreconditionerFactory();
  auto precond_ptr = factory[testParameters.precondSwitch]();
  FVCode3D::preconditioner & precond=*precond_ptr;
  loadPreconditioner(precond, saddlePointMat, testParameters);

  // Status
  int result{0};
  // timer
  Timings::Chrono timer;

 // Only to save typing I create an alias to testParameters
  auto maxit=testParameters.max_iter;
  auto tol  =testParameters.tol; 
  switch(testParameters.solverSwitch)
    {
    //case SparseLU :
      //{
        //Eigen::SparseLU<SpMat> SLUsolver;
        //SLUsolver.analyzePattern(A);
        //SLUsolver.factorize(A);
        //x= SLUsolver.solve(b);
        //maxit=0;
        //tol=0.0;
        //result=0;
      //}
      //break;
    case  umfpack:
      {
        Eigen::UmfPackLU<SpMat> UFsolver;
        SpMat A = saddlePointMat.fullMatrix();
        std::clog<<"size of full matrix:"<<A.rows()<<" "<<A.cols()<<std::endl;
        std::clog<<"Non zero entries   :"<<A.nonZeros()<<std::endl;
        UFsolver.analyzePattern(A);
        UFsolver.factorize(A);
        std::clog<<"Starting computations\n";
        timer.start();
        x= UFsolver.solve(b);
        timer.stop();
        std::clog<<timer;
	std::clog<<"End computations\n";
        maxit=0;
        tol=0.0;
        result=0;
      }
      break;
    case gmres :
      {
        auto m = testParameters.gmres_levels;
        std::clog<<"Starting computations\n";
        timer.start();
        result = GMRES(saddlePointMat, x, b, precond, m, maxit, tol);  // Solve system
        timer.stop();
        std::clog<<timer;
        std::clog<<"End computations\n";
        break;
      }
    case gmresr :
      {
        auto m = testParameters.gmres_levels;
        std::clog<<"Starting computations\n";
        timer.start();
        result = GMRESR(saddlePointMat, x, b, precond, m, maxit, tol);  // Solve system
        timer.stop();
        std::clog<<timer;
	std::clog<<"End computations\n";
        break;
      }
    case fgmres :
      {
	std::clog<<"Starting computations\n";
        timer.start();
        auto m = testParameters.gmres_levels;
        result = FGMRES(saddlePointMat, x, b, precond, m, maxit, tol);;  // Solve system
        timer.stop();
        std::clog<<timer;
	std::clog<<"End computations\n";
	break;
      }
    case minres:
      try
        {
	  std::clog<<"Starting computations\n";
	  timer.start();
	  result = MINRES(saddlePointMat, x, b, precond, maxit, tol);  // Solve system
	  timer.stop();
	  std::clog<<timer;
	  std::clog<<"End computations\n";
        }
      catch (std::runtime_error const & rte)
        {
          std::cerr<< "Something wrong "<<rte.what()<<std::endl;
        }
      break;
    case tminres:
	  std::clog<<"Starting computations\n";
	  timer.start();
	  result = TMINRES(saddlePointMat, x, b, precond, maxit, tol, true);   // Solve system
	  timer.stop();
	  std::clog<<timer;
	  std::clog<<"End computations\n";
	  break;
    case eigenvalues:
      break;
    };
  
  

  double solError=(x-e).norm()/e.norm();
  double resFinal=(b-saddlePointMat*x).norm()/b.norm();
  std::clog << "Solver flag = " << result << endl;
  std::clog << "iterations performed:  " << maxit << endl;
  std::clog << "tolerance achieved  :  " << tol << endl;
  std::clog << "Relative Error:        " << solError<<std::endl;
  std::clog << "Relative Residual Error:" << resFinal<<std::endl;
  std::clog << "Cond. Estimate         :"<< solError/resFinal<<std::endl;
  return result;
}
