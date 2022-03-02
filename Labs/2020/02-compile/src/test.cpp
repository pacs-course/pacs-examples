#include <stdlib.h>                         // System includes
//#include <iostream.h>                       // 
#include <iostream>                      // 
using std::endl;
using std::cin;
using std::cout;
using std::cerr;


#include <iostream>                      // 
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include <Eigen/UmfPackSupport>
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include "MM_readers.hpp"
#include "Utilities.hpp"
#include "iml++.hpp"                          // All IML++
#include <GetPot>
#include <map>
#include <string>
//! Select solvers
enum SolverSwitch {Umfpack, SparseLU, gmres, cg, cheby, bicgstab, cgs,bicg,ir,qmr};
//! where to store parameters
struct TestParameters
{
  double tol;
  int max_iter;
  int gmres_levels;
  double eigmin;
  double eigmax;
  std::string matrixFileName;
  SolverSwitch solverSwitch;
};

//! Map solvers to switch
const std::map<std::string,SolverSwitch> getSolver=
  {
    {std::string{"umfpack"},Umfpack},
    {std::string{"sparselu"},SparseLU},
    {std::string{"gmres"},gmres},
    {std::string{"cg"},cg},
    {std::string{"cheby"},cheby},
    {std::string{"bicgstab"},bicgstab},
    {std::string{"cgs"},cgs},
    {std::string{"bicg"},bicg},
    {std::string{"ir"},ir},
    {std::string{"qmr"},qmr}
  }; 

//! Read parameters from getpot object
TestParameters read_parameters(GetPot const & ifl)
{
  TestParameters r;
  r.tol            = ifl("tol", 1.0e-6);
  r.max_iter       = ifl("max_iter", 100);
  r.gmres_levels   = ifl("gmres_levels", 20);
  r.eigmin         = ifl("eigmin",1.0);
  r.eigmax         = ifl("eigmax",1.0);
  r.matrixFileName = ifl("matrixFileName","../../MatrixData/spd/Pres_Poisson.mtx");
  std::string  solver=ifl("solver","cg");
  auto pos         = ::getSolver.find(solver); //:: is not necessary
  if (pos==::getSolver.end())
    {
      throw std::runtime_error(std::string("FATAL ERROR: Solver ")+solver+std::string(" not found"));
    }
  r.solverSwitch   = pos->second;
  return r;
}
// It is useful to write the parameters just to check

std::ostream & operator << (std::ostream & out, TestParameters const & p)
{
  out<<"tol= "<< p.tol<<std::endl;
  out<<"max_iter= "<< p.max_iter<<std::endl;
  out<<"gmres_levels= "<< p.gmres_levels<<std::endl;
  out<<"eigmin= "<< p.eigmin<<std::endl;
  out<<"eigmax= "<< p.eigmax<<std::endl;
  out<<"matrix file name= "<< p.matrixFileName<<std::endl;
  // To get the solver is complicated because we have to
  // inverse the map: we are looking for the key corresponding to a
  // value. A nice use of lambdas
  auto s   = p.solverSwitch;
  // The type contained in the map is a pair of key and values
  using vt = std::pair<std::string,SolverSwitch>;
  // get the iterator to the solver
  auto res=std::find_if(getSolver.cbegin(),getSolver.cend(),[s](vt const & i){return i.second==s;});
  out<<"solver= "<< res->first<<std::endl;
  return out;
}

int
main(int argc, char * argv[])
{
  // Parse command line with getpot
  GetPot indata(argc,argv);
  if ( indata.search(2,"--help","-h") )
    {
      cout << "Usage: " << argv[0] << "<-i DataFile>" << endl;
      cout << "DataFile is a GetPot file. If missing is set to data.pot" << endl;
      return 0;
    };
  std::string dataFile;
  dataFile=indata.follow("data.pot","-i");
  std::cout<<" Reading data from "<<dataFile<<std::endl;
  // Read parameters from data
  GetPot ifl(dataFile.c_str());
  TestParameters testParameters=read_parameters(ifl);
  // Output parameters
  std::cout<<testParameters<<std::endl;
  // Some useful alias
  // Matrix is in column ordering
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;
  std::string matrixFile(testParameters.matrixFileName);
  SpMat A=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  // Just to make sure it is in a compressed mode
  A.makeCompressed();
  std::cout<<"Matrix size:"<<A.rows()<<"X"<<A.cols()<<std::endl;
  std::cout<<"Non zero entries:"<<A.nonZeros()<<std::endl;
  SpMat B = SpMat(A.transpose()) - A;
  std::cout<<"Norm of A-A.t: "<<B.norm()<<std::endl;
  //
  // Compute adjacency graph and print info.
  //
  LinearAlgebra::AdjacencyGraph graph(LinearAlgebra::extractCooData(A));
  graph.info(std::cout, false);
  //
  // Create the right hand side so that the solution is a vector of 1s
  //
  SpVec e=SpVec::Ones(A.rows());
  SpVec b=A* e;
  SpVec x(A.rows());
  /*
    If you wat to use ILU as preconditioner
    Eigen::IncompleteLUT<double> D;
    D.setDroptol(0.);
    D.setFillfactor(4);
    D.compute(A);
  */
  //Eigen::IdentityPreconditioner D(A);// Create identity preconditioner (no prec).
  Eigen::DiagonalPreconditioner<double> D(A);// Create diagonal preconditioner
  x=0*x;// Start from 0.

  // Status
  int result;

  // Only to save typing I create an alias to testParameters
  auto maxit=testParameters.max_iter;
  auto tol  =testParameters.tol; 
  switch(testParameters.solverSwitch)
    {
    case SparseLU :
      {
        Eigen::SparseLU<SpMat> SLUsolver;
        SLUsolver.analyzePattern(A);
        SLUsolver.factorize(A);
        x= SLUsolver.solve(b);
        maxit=0;
        tol=0.0;
        result=0;
      }
      break;
    case  Umfpack:
      {
        Eigen::UmfPackLU<SpMat> UFsolver;
        UFsolver.analyzePattern(A);
        UFsolver.factorize(A);
        x= UFsolver.solve(b);
        maxit=0;
        tol=0.0;
        result=0;
      }
      break;
    case gmres :
      {
        auto m = testParameters.gmres_levels;
        result = GMRES(A, x, b, D, m, maxit, tol);  // Solve system
        break;
      }
    case cg:
      result = CG(A, x, b, D, maxit, tol);   // Solve system
      break;
    case cheby:
      result = CHEBY(A, x, b, D, maxit, tol, testParameters.eigmin, testParameters.eigmax);  // Solve system
    break;
    case bicgstab:
      result = BiCGSTAB(A, x, b, D, maxit, tol); // Solve system
      break;
    case cgs:
      result = CGS(A, x, b, D, maxit, tol); // Solve system
      break;
    case bicg:
      result = BiCG(A, x, b, D, maxit, tol);   // Solve system
      break;
    case ir :
      result = IR(A, x, b, D, maxit, tol);   // Solve system
      break;
    case qmr:
      // We need left and right preconditioners. Here have been taken equal
      // for simplicity
      result = QMR(A, x, b, D, D, maxit, tol);  // Solve system
      break;
    };
  
  


  cout << "Solver flag = " << result << endl;
  cout << "iterations performed: " << maxit << endl;
  cout << "tolerance achieved  : " << tol << endl;
  cout << "Error:                " << (x-e).norm()<<std::endl;

  return result;
}
