//#include <stdlib.h>                         // System includes
//#include <iostream.h>                       //                   //
#include <iostream>
                        // All IML++
#include "GetPot"
#include "SaddlePointUtilities.hpp"
int
main(int argc, char * argv[])
{
  using namespace SaddlePointUtilities;
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
  std::cout<<"Matrix norm"<<saddlePointMat.norm()<<std::endl;

  //saddlePointMat.convertToDoubleSaddlePoint();
  //std::cout<<"Matrix norm"<<saddlePointMat.norm()<<std::endl;
  M.resize(0,0);
  B.resize(0,0);
  T.resize(0,0);
  M.data().squeeze();
  B.data().squeeze();
  T.data().squeeze();
// The saddlepoint matrix is composed with the block matrices (to be changed)
// free memory
  if (testParameters.solverSwitch==eigenvalues)
    {
      long int maxit=testParameters.EIGmaxit;
      long int ncv = testParameters.EIGncv;
      double eigtol = testParameters.EIGtol;
      Eigen::VectorXd evalues=computeSymEigenValues(saddlePointMat.getM(),2,ncv,maxit,eigtol);
      std::clog<<"Eigenvalues of M "<<evalues<<std::endl;
      std::clog<<"Condition Number of M "<<evalues(0)/evalues(evalues.size()-1)<<std::endl;
                   evalues=computeSymEigenValues(saddlePointMat.getT(),2,ncv,maxit,eigtol);
      std::clog<<"Eigenvalues of T "<<evalues<<std::endl;
      auto nn=saddlePointMat.squaredNorm();
      std::clog<<"Norm of SPM "<<std::sqrt(nn)<<" Normalised norm "<<std::sqrt(nn/saddlePointMat.nonZeros())<<std::endl;
      SpMat full{saddlePointMat.fullMatrix()};
      // free memory. I may need it!
      saddlePointMat.clear();
      evalues=computeSymEigenValues(full,2);
      std::clog<<"Smallest/largest Eigenvalues of A "<<evalues<<std::endl;
      //      evalues=computeSymEigenValues<SpMat,Spectra::SMALLEST_MAGN>(full,1,ncv,maxit,eigtol);
      //std::clog<<"Eigenvalue of A with smallest magnitude"<<evalues<<std::endl;
      return 0;
    }

  // Just to make sure it is in a compressed mode
  saddlePointMat.makeCompressed();
  // Create the right hand side so that the solution is a vector of 1s
  //
  SpVec e=SpVec::Random(saddlePointMat.rows());
  SpVec b=saddlePointMat * e;
  SpVec x(saddlePointMat.rows());

  x.fill(0.);// Start from 0.
  FVCode3D::PreconditionerFactory factory= FVCode3D::make_PreconditionerFactory();
  auto precond_ptr = factory[testParameters.precondSwitch]();
  FVCode3D::preconditioner & precond=*precond_ptr;

  if(testParameters.precondSwitch==FVCode3D::DoubleSaddlePoint || testParameters.precondSwitch==FVCode3D::DoubleSaddlePointSym)
    saddlePointMat.convertToDoubleSaddlePoint();

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
        UFsolver.umfpackReportInfo();
        std::clog<<"Starting computations\n";
        timer.start();
        x= UFsolver.solve(b);
        timer.stop();
        std::clog<<timer;
	std::clog<<"End computations\n";
        UFsolver.umfpackReportInfo();
        std::clog<<"Solution norm:"<<x.norm()<<"\n";
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
